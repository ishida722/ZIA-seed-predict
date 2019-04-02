/*
*------------------------------------------------------------
* Copyright(c) 2017 by Digital Media Professionals Inc.
* All rights reserved.
*------------------------------------------------------------
* This source code was generated using DMP-DV700 tools
* Digital Media Professionals Inc.
*------------------------------------------------------------
*/

#include <stdarg.h>

#include "dmp_network.h"


/// @brief Dummy layer to return on error (instantiation).
fpga_layer CDMP_Network::err_layer_;


#define LOG(...) fprintf(stdout, __VA_ARGS__); fflush(stdout);
#define ERR(...) fprintf(stderr, __VA_ARGS__); fflush(stderr);


void CDMP_Network::set_num_layers(int n) {
  if (num_layers_) {
    ERR("Double call of set_num_layers() detected\n");
    return;
  }
  num_layers_ = n;
  layers_.resize(num_layers_);
  memset(layers_.data(), 0, sizeof(layers_[0]) * layers_.size());
}


void CDMP_Network::set_num_output_layers(int n) {
  if (num_output_layers_) {
    ERR("Double call of set_num_output_layers() detected\n");
    return;
  }
  num_output_layers_ = n;
  output_layers_.resize(num_output_layers_);
  memset(output_layers_.data(), 0, sizeof(output_layers_[0]) * output_layers_.size());
}


fpga_layer& CDMP_Network::get_layer(int i) {
  if ((i < 0) || (i >= (int)layers_.size())) {
    LOG("Requested layer index is out of bounds: %d\n", i);
    return err_layer_;
  }
  return layers_[i];
}


bool CDMP_Network::ReserveMemory(size_t weights_size, size_t io_size) {
  if ((ctx_) || (weights_mem_) || (io_mem_)) {
    ERR("Double call of ReserveMemory()\n");
    return false;
  }

  ctx_ = dmp_dv_context_create();
  if (!ctx_) {
    ERR("Unable to create DV context: %s\n", dmp_dv_get_last_error_message());
    return false;
  }

  memset(&dv_info_, 0, sizeof(dv_info_));
  dv_info_.header.size = sizeof(dv_info_);
  dv_info_.header.version = 0;
  if (dmp_dv_context_get_info(ctx_, (dmp_dv_info*)&dv_info_)) {
    ERR("Unable to get information about DV context: %s\n", dmp_dv_get_last_error_message());
    return false;
  }

  weights_size_ = weights_size;
  io_size_ = io_size;

  weights_mem_ = dmp_dv_mem_alloc(ctx_, weights_size_);
  if (!weights_mem_) {
    ERR("Failed to allocate %zu bytes for DV accelerator: %s\n",
        weights_size_, dmp_dv_get_last_error_message());
    return false;
  }
  if (iprint_ > 0) {
    LOG("Allocated %zu bytes of memory for weights (%zu requested)\n",
        dmp_dv_mem_get_size(weights_mem_), weights_size_);
  }

  io_mem_ = dmp_dv_mem_alloc(ctx_, io_size_);
  if (!io_mem_) {
    ERR("Failed to allocate %zu bytes for DV accelerator: %s\n",
        io_size_, dmp_dv_get_last_error_message());
    return false;
  }
  if (iprint_ > 0) {
    LOG("Allocated %zu bytes of memory for input/output (%zu requested)\n",
        dmp_dv_mem_get_size(io_mem_), io_size_);
  }

  io_ptr_ = dmp_dv_mem_map(io_mem_);
  if (!io_ptr_) {
    ERR("Failed to map %zu bytes of input/output buffer into application's virtual memory: %s\n",
        dmp_dv_mem_get_size(io_mem_), dmp_dv_get_last_error_message());
    return false;
  }

  if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
    ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
    return false;
  }
  memset(io_ptr_, 0, dmp_dv_mem_get_size(io_mem_));

  return true;
}


bool CDMP_Network::LoadWeights(const std::string& filename) {
  if (!weights_mem_) {
    ERR("Weights buffer is NULL, ReserveMemory() must be called and succeed first\n");
    return false;
  }

  FILE *fin = fopen(filename.c_str(), "rb");
  if (!fin) {
    ERR("Failed to open weights file: %s\n", filename.c_str());
    return false;
  }

  uint8_t *buf = dmp_dv_mem_map(weights_mem_);
  if (!buf) {
    ERR("Failed to map %zu bytes of weights buffer into application's virtual memory: %s\n",
        dmp_dv_mem_get_size(weights_mem_), dmp_dv_get_last_error_message());
    fclose(fin);
    return false;
  }
  if (dmp_dv_mem_sync_start(weights_mem_, 0, 1)) {
    ERR("Failed to start CPU->Device memory synchronization for %zu bytes of weights memory: %s\n",
        dmp_dv_mem_get_size(weights_mem_), dmp_dv_get_last_error_message());
    fclose(fin);
    return false;
  }
  size_t n = fread(buf, 1, weights_size_, fin);
  if (n != weights_size_) {
    ERR("Incomplete read of weights from %s: read %zu bytes instead of %zu\n",
        filename.c_str(), n, weights_size_);
    fclose(fin);
    return false;
  }
  char c;
  if ((fread(&c, 1, 1, fin) != 0) || (!feof(fin))) {
    ERR("File with weights %s is larger than computed weights size %zu\n",
        filename.c_str(), weights_size_);
    fclose(fin);
    return false;
  }
  fclose(fin);
  dmp_dv_mem_unmap(weights_mem_);

  weights_loaded_ = true;

  if (iprint_ > 0) {
    LOG("Loaded %zu bytes for weights from %s\n",
        weights_size_, filename.c_str());
  }

  return true;
}


bool CDMP_Network::Commit() {
  return GenerateCommandLists();
}


/// @brief Reorders channels from WHC8 to HWC.
static void remap(uint16_t* __restrict src, uint16_t* __restrict dst, int x_size, int y_size, int channel_size) {
  for (int y = 0; y < y_size; ++y) {
    for (int x = 0; x < x_size; ++x) {
      for (int i = 0; i < channel_size; i += 8) {
        const int copy_size = std::min(channel_size - i, 8);
        memcpy(dst + (y * x_size + x) * channel_size + i,
               src + i * (x_size * y_size) + (x * y_size + y) * copy_size,
               copy_size * sizeof(uint16_t));
      }
    }
  }
}


/// @brief Reorders channels from HWC to WHC8.
static void remap_hw(uint16_t* __restrict src, uint16_t* __restrict dst, int x_size, int y_size, int channel_size) {
  for (int y = 0; y < y_size; ++y) {
    for (int x = 0; x < x_size; ++x) {
      for (int i = 0; i < channel_size; i += 8) {
        const int copy_size = std::min(channel_size - i, 8);
        memcpy(dst + i * (x_size * y_size) + (x * y_size + y) * copy_size,
               src + (y * x_size + x) * channel_size + i,
               copy_size * sizeof(uint16_t));
      }
    }
  }
}


/// @brief Converts array of half precision floats to single precision.
static void fp16_to_float(float* __restrict dst, const __fp16* __restrict src, int n) {
  for (int i = 0; i < n; ++i) {
    dst[i] = (float)src[i];
  }
}


/// @brief Converts array of single precision floats to half precision.
static void float_to_fp16(__fp16* __restrict dst, const float* __restrict src, int n) {
  for (int i = 0; i < n; ++i) {
    dst[i] = (__fp16)src[i];
  }
}


/// @brief Runs softmax operation on CPU.
static void run_softmax(fpga_layer& layer, int softmax_axis, uint8_t *io_ptr) {
  void *src_buffer = io_ptr + layer.input_offs;
  void *dst_buffer = io_ptr + layer.output_offs;

  if (layer.is_input_hw_layout) {
    uint16_t *src = (uint16_t*)src_buffer;
    uint16_t *dst = (uint16_t*)dst_buffer;
    int x_size = layer.input_dim[0];
    int y_size = layer.input_dim[1];
    int channel_size = layer.input_dim[2];
    dst += x_size * y_size * channel_size;
    remap(src, dst, x_size, y_size, channel_size);
    src_buffer = dst;
  }

  int tensor_size = 1, group_size, axis_size, remain_size;
  int axis_stride = 1, group_stride;
  for (int i = 0; i < layer.input_dim_size; i++) {
    tensor_size *= layer.input_dim[i];
    if (i > softmax_axis) {
      axis_stride *= layer.input_dim[i];
    }
    axis_size = layer.input_dim[softmax_axis];
    remain_size = axis_stride;
    group_size = tensor_size / axis_size / remain_size;
    group_stride = axis_size * axis_stride;
    fp16_to_float((float*)dst_buffer, (__fp16*)src_buffer, tensor_size);

    for (int j = 0; j < group_size; ++j) {
      for (int k = 0; k < remain_size; ++k) {
        float *f = (float*)dst_buffer + (j * group_stride + k);
        float fmax = f[0];
        for (int i = 1; i < axis_size; ++i) {
          fmax = std::max(fmax, f[i * axis_stride]);
        }
        for (int i = 0; i < axis_size; ++i) {
          f[i * axis_stride] -= fmax;
        }
        float e_sum = 0;
        for (int i = 0; i < axis_size; ++i) {
          float d = std::exp(f[i * axis_stride]);
          f[i * axis_stride] = d;
          e_sum += d;
        }
        if(std::fabs(e_sum) < 1e-6f) {
          ERR("SoftMax implementaion error, the sum of exp is too low: %.6e\n", e_sum);
        }

        float inv_e_sum = 1.0f / e_sum;
        for (int i = 0; i < axis_size; i++) {
          f[i * axis_stride] *= inv_e_sum;
        }
      }
    }
  }
}


/// @brief Executes flatten operation.
static void run_flatten(fpga_layer &layer, uint8_t *io_ptr) {
  if (!layer.is_input_hw_layout) {
    return;
  }
  uint16_t *src = (uint16_t*)(io_ptr + layer.input_offs);
  uint16_t *dst = (uint16_t*)(io_ptr + layer.output_offs);
  int x_size = layer.input_dim[0];
  int y_size = layer.input_dim[1];
  int channel_size = layer.input_dim[2];
  remap(src, dst, x_size, y_size, channel_size);
}


/// @brief Executes concatenation via memory copy.
static void run_copy_concat(fpga_layer& layer, int input_layer_num, fpga_layer **input_layers, uint8_t *io_ptr) {
  const int chunk_size = 8;
  uint16_t *dst = (uint16_t*)(io_ptr + layer.output_offs);
  const int x_size = layer.output_dim[0];
  const int y_size = layer.output_dim[1];
  const int dst_channel_size = layer.output_dim[2];
  const int chunk_stride = x_size * y_size * chunk_size;
  int dst_copied_size = 0;
  for (int i = 0; i < input_layer_num; ++i) {
    uint16_t *src = (uint16_t*)(io_ptr + input_layers[i]->output_offs);
    const int src_channel_size = input_layers[i]->output_dim[2];
    int src_copied_size = 0;
    while (src_copied_size < src_channel_size) {
      int dst_copy_size = chunk_size - (dst_copied_size % chunk_size);
      if (dst_copy_size > dst_channel_size - dst_copied_size) {
        dst_copy_size = dst_channel_size - dst_copied_size;
      }
      int src_copy_size = chunk_size - (src_copied_size % chunk_size);
      if (src_copy_size > src_channel_size - src_copied_size) {
        src_copy_size = src_channel_size - src_copied_size;
      }
      int copy_size = (dst_copy_size < src_copy_size ? dst_copy_size : src_copy_size);
      uint16_t *dst_copy = dst + (dst_copied_size / chunk_size) * chunk_stride + (dst_copied_size % chunk_size);
      uint16_t *src_copy = src + (src_copied_size / chunk_size) * chunk_stride + (src_copied_size % chunk_size);
      const int dst_stride = (dst_channel_size / chunk_size > dst_copied_size / chunk_size ? chunk_size : dst_channel_size % chunk_size);
      const int src_stride = (src_channel_size / chunk_size > src_copied_size / chunk_size ? chunk_size : src_channel_size % chunk_size);
      for (int j = 0; j < x_size * y_size; ++j) {
        memcpy(dst_copy, src_copy, sizeof(uint16_t) * copy_size);
        dst_copy += dst_stride;
        src_copy += src_stride;
      }
      dst_copied_size += copy_size;
      src_copied_size += copy_size;
    }
  }
}


bool CDMP_Network::RunNetwork() {
  if ((!weights_loaded_) && (weights_size_)) {
    ERR("LoadWeights() must be called first");
    return false;
  }

  TimeInterval dt;
  int64_t exec_id = -1;
  dmp_dv_cmdlist cmdlist = NULL;
  int conv_usec = 0;
  int fc_usec = 0;
  int cpu_usec = 0;

  const int n_layers = (int)layers_.size();
  for (int i_layer = 0; i_layer < n_layers; ++i_layer) {
    fpga_layer& layer = layers_[i_layer];
    if (layer.cmdlist) {
      // Execute command list on first command
      if (layer.cmdlist_pos == 0) {
        if (exec_id >= 0) {
          ERR("Incorrect layers order detected: layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d, but exec_id is already set\n",
              i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size);
          return false;
        }
        if (dmp_dv_mem_sync_end(io_mem_)) {
          ERR("Failed to end synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
          return false;
        }
        dt.reset();  // start time measurement
        exec_id = dmp_dv_cmdlist_exec(layer.cmdlist);
        if (exec_id < 0) {
          ERR("Could not execute command list for layer %d, name=%s: %s\n",
              i_layer, layer.name.c_str(), dmp_dv_get_last_error_message());
          return false;
        }
        if (iprint_ > 1) {
          LOG("Executed command list for layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d, exec_id=%lld\n",
              i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size, (long long)exec_id);
        }
        cmdlist = layer.cmdlist;  // remember last executed command list
      }

      // Wait for command list completion on last command
      if (layer.cmdlist_pos == layer.cmdlist_size - 1) {
        if (exec_id < 0) {
          ERR("Incorrect layers order detected: layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d, but exec_id is not set\n",
              i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size);
          return false;
        }
        if (dmp_dv_cmdlist_wait(cmdlist, exec_id)) {
          ERR("Wait for command completion failed, issued on layer %d, name=%s: %s\n",
              i_layer, layer.name.c_str(), dmp_dv_get_last_error_message());
          return false;
        }
        const int usec = dt.get_us();
        switch (layer.type) {
          case LT_CONV:
            conv_usec += usec;
            break;
          case LT_FC:
            fc_usec += usec;
            break;
          default:
            ERR("Possible implementation error on line %d of file %s: layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d\n",
                __LINE__, __FILE__, i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size);
            return false;
        }
        if (iprint_ > 1) {
          LOG("Waited on command list for layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d, exec_id=%lld, usec=%d\n",
              i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size, (long long)exec_id, usec);
        }
        exec_id = -1;
        cmdlist = NULL;
      }
      if (layer.cmdlist_pos >= layer.cmdlist_size) {
        ERR("Possible implementation error on line %d of file %s: layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d\n",
            __LINE__, __FILE__, i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size);
        return false;
      }
    }
    else if (exec_id >= 0) {
      ERR("Incorrect layers order detected: layer %d, name=%s cmdlist_pos=%d cmdlist_size=%d, but exec_id is already set\n",
          i_layer, layer.name.c_str(), layer.cmdlist_pos, layer.cmdlist_size);
      return false;
    }
    switch (layer.type) {
      case LT_CONV:
      case LT_FC:
        if (!layer.cmdlist) {
          ERR("Command list for layer %d, name=%s is NULL, Initialize() must be called and succeed first\n",
              i_layer, layer.name.c_str());
          return false;
        }
        break;
      case LT_SOFTMAX:
        if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
          ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
          return false;
        }
        dt.reset();
        run_softmax(layer, layer.softmax_axis, io_ptr_);
        cpu_usec += dt.get_us();
        break;
      case LT_FLATTEN:
        if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
          ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
          return false;
        }
        dt.reset();
        run_flatten(layer, io_ptr_);
        cpu_usec += dt.get_us();
        break;
      case LT_COPY_CONCAT:
        if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
          ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
          return false;
        }
        dt.reset();
        run_copy_concat(layer, layer.input_layer_num, layer.input_layers, io_ptr_);
        cpu_usec += dt.get_us();
        break;
      case LT_CUSTOM:
        if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
          ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
          return false;
        }
        dt.reset();
        (*(layer.custom_proc_ptr))(layer, layer.custom_param, io_ptr_);
        cpu_usec += dt.get_us();
        break;
      case LT_INPUT:
      case LT_CONCAT:
        // Layer does nothing by design
        break;
    }

    if (want_layer_outputs_) {
      if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
        ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
        return false;
      }
      layer.output.resize(layer.output_size >> 1);
      memcpy(layer.output.data(), io_ptr_ + layer.output_offs, (layer.output_size >> 1) << 1);
    }
  }

  last_conv_usec_ = conv_usec;
  last_fc_usec_ = fc_usec;
  last_cpu_usec_ = cpu_usec;

  if (dmp_dv_mem_sync_start(io_mem_, 1, 1)) {
    ERR("Failed to start synchronization on memory for input/output: %s", dmp_dv_get_last_error_message());
    return false;
  }

  return true;
}


void* CDMP_Network::get_network_input_addr_cpu() {
  return io_ptr_ + layers_[0].input_offs;
}


void CDMP_Network::get_final_output(std::vector<float>& out, int i_output) {
  int output_size = output_layers_[i_output]->output_size;
  if (output_layers_[i_output]->is_f32_output) {
    output_size >>= 2;
  }
  else {
    output_size >>= 1;
  }

  if((int)out.size() != output_size) {
    out.resize(output_size);
  }

  if (output_layers_[i_output]->is_f32_output) {
    memcpy(out.data(), io_ptr_ + output_layers_[i_output]->output_offs, output_size << 2);
  }
  else {
    fp16_to_float(out.data(), (__fp16*)(io_ptr_ + output_layers_[i_output]->output_offs), output_size);
  }
}


void get_layer_input(fpga_layer &layer, std::vector<float> &layer_input, uint8_t *io_ptr) {
    int input_size = 1;
    for (int i = 0; i < layer.input_dim_size; ++i) {
      input_size *= layer.input_dim[i];
    }

    if((int)layer_input.size() != input_size) {
      layer_input.resize(input_size);
    }

    if (layer.is_input_hw_layout) {
      uint16_t *src = (uint16_t*)(io_ptr + layer.input_offs);
      uint16_t *dst = (uint16_t*)&layer_input.front();
      int x_size = layer.input_dim[0];
      int y_size = layer.input_dim[1];
      int channel_size = layer.input_dim[2];
      dst += input_size;
      remap(src, dst, x_size, y_size, channel_size);
      fp16_to_float((float*)layer_input.data(), (__fp16*)dst, input_size);
    }
    else {
      fp16_to_float((float*)layer_input.data(), (__fp16*)(io_ptr + layer.input_offs), input_size);
    }
}


void put_layer_output(fpga_layer& layer, std::vector<float>& layer_output, uint8_t *io_ptr, bool is_output_hw_layout) {
  int output_size = 1;
  for (int i = 0; i < layer.output_dim_size; ++i) {
    output_size *= layer.output_dim[i];
  }

  if (!is_output_hw_layout) {
    memcpy(io_ptr + layer.output_offs, (void*)(&layer_output.front()), layer.output_size);
  }
  else {
    float *src = (float*)&layer_output.front();
    __fp16 *dst = (__fp16*)(io_ptr + layer.output_offs);
    int x_size = layer.output_dim[0];
    int y_size = layer.output_dim[1];
    int channel_size = layer.output_dim[2];
    dst += output_size;
    float_to_fp16(dst + output_size, src, output_size);
    remap_hw((uint16_t*)dst + output_size, (uint16_t*)dst, x_size, y_size, channel_size);
  }
}


bool CDMP_Network::GenerateCommandLists() {
  if (cmdlists_.size()) {
    ERR("Double call of GenerateCommandLists() detected\n");
    return false;
  }

  // Create command lists
  layer_type last_type = LT_INPUT;
  int i_layer = 0;
  for (auto it = layers_.begin(); it != layers_.end(); ++it, ++i_layer) {
    switch (it->type) {
      case LT_CONV:
        if ((want_layer_outputs_) || (last_type != LT_CONV)) {
          dmp_dv_cmdlist cmdlist = dmp_dv_cmdlist_create(ctx_);
          if (!cmdlist) {
            ERR("dmp_dv_cmdlist_create() failed: %s\n", dmp_dv_get_last_error_message());
            return false;
          }
          cmdlists_.push_back(cmdlist);
        }
        it->cmdlist = cmdlists_.back();
        if (dmp_dv_cmdlist_add_raw(it->cmdlist, (dmp_dv_cmdraw*)&it->conv_conf)) {
          ERR("dmp_dv_cmdlist_add_raw() failed: %s\n", dmp_dv_get_last_error_message());
          return false;
        }
        break;
      case LT_FC:
        if ((want_layer_outputs_) || (last_type != LT_FC)) {
          dmp_dv_cmdlist cmdlist = dmp_dv_cmdlist_create(ctx_);
          if (!cmdlist) {
            ERR("dmp_dv_cmdlist_create() failed: %s\n", dmp_dv_get_last_error_message());
            return false;
          }
          cmdlists_.push_back(cmdlist);
        }
        it->cmdlist = cmdlists_.back();
        if (dmp_dv_cmdlist_add_raw(it->cmdlist, (dmp_dv_cmdraw*)&it->fc_conf)) {
          ERR("dmp_dv_cmdlist_add_raw() failed: %s\n", dmp_dv_get_last_error_message());
          return false;
        }
        break;
      default:
        // Empty by design
        break;
    }
    last_type = it->type;
  }

  if (iprint_ > 1) {
    LOG("Generated %d command lists\n", (int)cmdlists_.size());
  }

  // Assign cmdlist_pos and cmdlist_size
  dmp_dv_cmdlist last_cmdlist = NULL;
  int last_pos = 0;
  const int n_layers = (int)layers_.size();
  for (int i = 0; i < n_layers; ++i) {
    fpga_layer& layer = layers_[i];
    if (layer.cmdlist == last_cmdlist) {
      if (last_cmdlist) {
        layer.cmdlist_pos = last_pos++;
      }
    }
    if (((layer.cmdlist != last_cmdlist) || (i == n_layers - 1)) && (last_cmdlist)) {
      for (int j = i - 1; j >= 0; --j) {
        if (layers_[j].cmdlist != last_cmdlist) {
          break;
        }
        layers_[j].cmdlist_size = last_pos;
      }
      if (i == n_layers - 1) {
        layer.cmdlist_size = last_pos;
      }
      else {
        last_pos = 0;
        if (layer.cmdlist) {
          layer.cmdlist_pos = last_pos++;
        }
      }
    }
    else if ((!last_cmdlist) && (layer.cmdlist)) {
      last_pos = 0;
      layer.cmdlist_pos = last_pos++;
      if (i == n_layers - 1) {
        layer.cmdlist_size = last_pos;
      }
    }
    last_cmdlist = layer.cmdlist;
  }

  // Commit command lists
  int n_commited = 0;
  for (auto it = cmdlists_.begin(); it != cmdlists_.end(); ++it) {
    if (dmp_dv_cmdlist_commit(*it)) {
      ERR("Could not commit command list: %s\n", dmp_dv_get_last_error_message());
      return false;
    }
    n_commited += 1;
  }

  return true;
}
