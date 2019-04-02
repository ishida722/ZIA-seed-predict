/*
 *  Copyright 2018 Digital Media Professionals Inc.

 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at

 *      http://www.apache.org/licenses/LICENSE-2.0

 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*
 * @brief Helpers for feed forward neural network specification.
 */
#pragma once

#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <stdint.h>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <ostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cmath>

#include "stats.h"
#include "dmp_dv.h"
#include "dmp_dv_cmdraw_v0.h"


/// @brief Layer type enumeration.
enum layer_type {
  LT_INPUT,
  LT_CONV,
  LT_FC,
  LT_FLATTEN,
  LT_CONCAT,
  LT_COPY_CONCAT,
  LT_SOFTMAX,
  LT_CUSTOM,
};


/// @brief Forward reference for layer specification.
struct fpga_layer;


/// @brief Callback.
/// @param layer Reference to layer specification.
/// @param custom_param User-specified parameter.
/// @param io_ptr Pointer to the base of input/output buffer.
typedef void (*run_custom_callback_proc)(fpga_layer& layer, void *custom_param, uint8_t *io_ptr);


/// @brief Layer specification.
struct fpga_layer {
  layer_type type;   // layer type
  std::string name;  // layer name
  union {
    struct dmp_dv_cmdraw_conv_v0 conv_conf;  // convolutional configuration
    struct dmp_dv_cmdraw_fc_v0 fc_conf;      // fully connected configuration
    int softmax_axis;  // softmax configuration
    struct {
      int input_layer_num;
      fpga_layer **input_layers;
    };  // copy concat configuration
    struct {
      run_custom_callback_proc custom_proc_ptr;
      void *custom_param;
    };  // generic CPU layer configuration
  };
  size_t input_offs;     // offset for an input from the base of the input/output buffer
  size_t output_offs;    // offset for an output from the base of the input/output buffer
  size_t output_size;    // size of the output in bytes
  int input_dim[3];      // input dimensions: HWC
  int input_dim_size;    // number of input dimensions
  int output_dim[3];     // output dimensions: HWC
  int output_dim_size;   // number of output dimensions
  std::vector<__fp16> output;  // raw layer output if WantLayerOutput() was ever called

  bool is_output;           // is the layer output
  bool is_f32_output;       // is the output in 32-bit float format
  bool is_input_hw_layout;  // is the input in WHC8 format

  dmp_dv_cmdlist cmdlist;   // command list containing this layer, can be NULL
  int cmdlist_pos;          // position of this layer in the command list
  int cmdlist_size;         // size of the command list
};


/// @brief Base helper class for feed forward neural network specification.
class CDMP_Network {
 public:
  /// @brief Constructor.
  CDMP_Network() {
    iprint_ = 0;
    num_layers_ = 0;
    num_output_layers_ = 0;
    weights_size_ = 0;
    io_size_ = 0;

    ctx_ = NULL;
    memset(&dv_info_, 0, sizeof(dv_info_));
    weights_mem_ = NULL;
    io_mem_ = NULL;
    io_ptr_ = NULL;

    last_conv_usec_ = 0;
    last_fc_usec_ = 0;
    last_cpu_usec_ = 0;

    weights_loaded_ = false;
    want_layer_outputs_ = false;
  };

  /// @brief Destructor.
  virtual ~CDMP_Network() {
    ReleaseResources();
  };

  /// @brief Returns reference to output layer definition.
  inline std::vector<fpga_layer*>& get_output_layers() {
    return output_layers_;
  }

  /// @brief Returns reference to the specific layer definition.
  fpga_layer& get_layer(int i);

  /// @brief Returns total number of layers.
  inline int get_total_layer_count() const {
    return num_layers_;
  }

  /// @brief Returns number of output layers.
  inline int get_output_layer_count() const {
    return num_output_layers_;
  }

  /// @brief Initializes the network.
  virtual bool Initialize() = 0;

  /// @brief Loads packed weights from file.
  bool LoadWeights(const std::string& filename);

  /// @brief Commits the network configuration.
  /// @details Must be called after LoadWeights if network contains fully connected layer.
  bool Commit();

  /// @brief Runs the network.
  bool RunNetwork();

  /// @brief Returns address of the input buffer of the first layer.
  void *get_network_input_addr_cpu();

  /// @brief Copies output data to the provided buffer in single-precision float format.
  void get_final_output(std::vector<float>& output, int i_output = 0);

  /// @brief Returns last time spent for executing convolutional layers in microseconds.
  inline int get_conv_usec() const {
    return last_conv_usec_;
  }

  /// @brief Returns last time spent for fully connected layers in microseconds.
  inline int get_fc_usec() const {
    return last_fc_usec_;
  }

  /// @brief Returns last time spent for CPU layers in microseconds.
  inline int get_cpu_usec() const {
    return last_cpu_usec_;
  }

  /// @brief Sets verbosity level.
  inline void Verbose(int level) {
    iprint_ = level;
  }

  /// @brief Returns pointer to the base of input/output buffer.
  inline uint8_t *get_io_ptr() const {
    return io_ptr_;
  }

  /// @brief Returns reference to information about DV context.
  inline const dmp_dv_info_v0& get_dv_info() const {
    return dv_info_;
  }

  /// @brief Output of all layers will be copied.
  /// @details Make every layer to be executed separately and it's output to be copied to fpga_layer.output.
  ///          Considerably slows down the execution, mainly for debugging purposes.
  ///          Should be called before Initialize().
  inline void WantLayerOutputs() {
    want_layer_outputs_ = true;
  }

 protected:
  /// @brief Reserves memory for weights and input/output.
  /// @details Must be called at the beginning of Initialize() in the derived class.
  bool ReserveMemory(size_t weights_size, size_t io_size);

  /// @brief Creates required number of command lists and assigns "cmdlist*" fileds in layers_.
  /// @details Must be called at the end of Initialize() in the derived class.
  bool GenerateCommandLists();

  /// @brief Sets total number of layers.
  /// @details Must be called once in Initialize() in the derived class.
  void set_num_layers(int n);

  /// @brief Sets number of output layers.
  /// @details Must be called once in Initialize() in the derived class.
  void set_num_output_layers(int n);

  /// @brief Number of layers.
  int num_layers_;

  /// @brief Number of output layers.
  int num_output_layers_;

  /// @brief Layers description in order of execution.
  std::vector<fpga_layer> layers_;

  /// @brief Pointers to output layers.
  std::vector<fpga_layer*> output_layers_;

  /// @brief Context.
  dmp_dv_context ctx_;

  /// @brief Information about device.
  struct dmp_dv_info_v0 dv_info_;

  /// @brief Memory allocation for weights.
  dmp_dv_mem weights_mem_;

  /// @brief Memory allocation for input/output.
  dmp_dv_mem io_mem_;

  /// @brief Memory pointer to the beginning of input/output buffer.
  uint8_t *io_ptr_;

  /// @brief Command lists for executing different sets of layers.
  std::vector<dmp_dv_cmdlist> cmdlists_;

  /// @brief Last time spent on all convolutional layers in microseconds.
  int last_conv_usec_;

  /// @brief Last time spent on all fully connected layers in microseconds.
  int last_fc_usec_;

  /// @brief Last time spent on all layers executed on CPU in microseconds.
  int last_cpu_usec_;

 private:
  /// @brief Releases held resources.
  void ReleaseResources() {
    for (auto it = cmdlists_.rbegin(); it != cmdlists_.rend(); ++it) {
      dmp_dv_cmdlist_release(*it);
    }
    cmdlists_.clear();
    dmp_dv_mem_release(io_mem_);
    dmp_dv_mem_release(weights_mem_);
    dmp_dv_context_release(ctx_);

    output_layers_.clear();
    layers_.clear();

    num_layers_ = 0;
    num_output_layers_ = 0;
    weights_size_ = 0;
    io_size_ = 0;

    ctx_ = NULL;
    weights_mem_ = NULL;
    io_mem_ = NULL;
    io_ptr_ = NULL;

    last_conv_usec_ = 0;
    last_fc_usec_ = 0;
    last_cpu_usec_ = 0;
  }

  /// @brief Size of weights buffer in bytes.
  size_t weights_size_;

  /// @brief Size of input/output buffer in bytes.
  size_t io_size_;

  /// @brief Verbosity level.
  int iprint_;

  /// @brief Dummy layer to return on error.
  static fpga_layer err_layer_;

  /// @brief Flag that LoadWeights was called.
  bool weights_loaded_;

  /// @brief Save a copy of output of each layer.
  bool want_layer_outputs_;
};


/// @brief Fills provided vector with the input data for the specified layer.
void get_layer_input(fpga_layer& layer, std::vector<float>& layer_input, uint8_t *io_ptr);


/// @brief Fills provided vector with the output data for the specified layer.
/// @param layer Layer to retrieve output from.
/// @param layer_output Vector to put requested data into.
/// @param is_output_hw_layout Whether data should be put in HW-specific layout (WHC8) or in Caffe-style CHW.
void put_layer_output(fpga_layer& layer, std::vector<float>& layer_output, uint8_t *io_ptr, bool is_output_hw_layout = false);
