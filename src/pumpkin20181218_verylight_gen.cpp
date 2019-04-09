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

 *  This source code was generated using DMP-DV700 tools.
 */

#include "pumpkin20181218_verylight_gen.h"


Cpumpkin20181218_verylight::Cpumpkin20181218_verylight() {
  // Empty by design
}

Cpumpkin20181218_verylight::~Cpumpkin20181218_verylight() {
  // Empty by design
}

bool Cpumpkin20181218_verylight::Initialize() {
  if (!ReserveMemory(16859600, 133152)) {
    return false;
  }

  set_num_layers(6);
  set_num_output_layers(1);

  Layer_0();
  Layer_1();
  Layer_2();
  Layer_3();
  Layer_4();
  Layer_5();

  return true;
}

//Layer_0: Convolution Layer
//  ->: conv2d_1
//  ->: activation_1
//  ->: max_pooling2d_1
void Cpumpkin20181218_verylight::Layer_0() {
  dmp_dv_cmdraw_conv_v0& conf = get_layer(0).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 64;  // Input Width
  conf.h = 64;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 1;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 67616;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv2d_1
  //->: activation_1
  //->: max_pooling2d_1
  conf.run[0].m = 32;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x5;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 0;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x202;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(0);
  layer.name = "max_pooling2d_1";
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 67616;
  layer.output_size = 65536;
  layer.input_dim[0] = 64;
  layer.input_dim[1] = 64;
  layer.input_dim[2] = 1;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 32;
  layer.output_dim[1] = 32;
  layer.output_dim[2] = 32;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = false;
}//end of  Layer_0

//Layer_1: Convolution Layer
//  ->: conv2d_2
//  ->: activation_2
//  ->: max_pooling2d_2
void Cpumpkin20181218_verylight::Layer_1() {
  dmp_dv_cmdraw_conv_v0& conf = get_layer(1).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 32;  // Input Width
  conf.h = 32;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 32;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 67616;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 34848;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv2d_2
  //->: activation_2
  //->: max_pooling2d_2
  conf.run[0].m = 64;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x5;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 2880;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x202;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(1);
  layer.name = "max_pooling2d_2";
  layer.type = LT_CONV;
  layer.input_offs = 67616;
  layer.output_offs = 34848;
  layer.output_size = 32768;
  layer.input_dim[0] = 32;
  layer.input_dim[1] = 32;
  layer.input_dim[2] = 32;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 16;
  layer.output_dim[1] = 16;
  layer.output_dim[2] = 64;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_1

//Layer_2: Flatten Layer
//	->: flatten_1
void Cpumpkin20181218_verylight::Layer_2() {
  fpga_layer& layer = get_layer(2);
  layer.name = "flatten_1";
  layer.type = LT_FLATTEN;
  layer.input_offs = 34848;
  layer.output_offs = 2080;
  layer.output_size = 32768;
  layer.input_dim[0] = 16;
  layer.input_dim[1] = 16;
  layer.input_dim[2] = 64;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 16384;
  layer.output_dim_size = 1;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_2

// Layer_3: Fully Connected Layer
//	->: dense_1
void Cpumpkin20181218_verylight::Layer_3() {
  dmp_dv_cmdraw_fc_v0& conf = get_layer(3).fc_conf;
  conf.header.size = sizeof(conf);
  conf.header.version = 0;
  conf.header.device_type = DMP_DV_DEV_FC;
  conf.input_size = 16384;
  conf.output_size = 1024;
  conf.weight_buf.mem = weights_mem_;
  conf.weight_buf.offs = 77248;
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 2080;
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 32;
  conf.weight_fmt = 1;  // 0 = unquantized weight matrix, 1 = qunatized
  conf.actfunc = 1;  // Activation Function: 0 = None, 1 = ReLU, 2 = Tanh, 3 = Leaky ReLU, 4 = Sigmoid, 5 = PReLU (PReLU must be used with POST-OP=1)
  conf.actfunc_param = 0x0;  // Leaky ReLU parameter (in FP16 format), 0 = non-leaky

  fpga_layer& layer = get_layer(3);
  layer.name = "dense_1";
  layer.type = LT_FC;
  layer.input_offs = 2080;
  layer.output_offs = 32;
  layer.output_size = 2048;
  layer.input_dim[0] = 16384;
  layer.input_dim_size = 1;
  layer.output_dim[0] = 1024;
  layer.output_dim_size = 1;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = false;
}//end of  Layer_3

// Layer_4: Fully Connected Layer
//	->: dense_2
void Cpumpkin20181218_verylight::Layer_4() {
  dmp_dv_cmdraw_fc_v0& conf = get_layer(4).fc_conf;
  conf.header.size = sizeof(conf);
  conf.header.version = 0;
  conf.header.device_type = DMP_DV_DEV_FC;
  conf.input_size = 1024;
  conf.output_size = 2;
  conf.weight_buf.mem = weights_mem_;
  conf.weight_buf.offs = 16857024;
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 32;
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 16;
  conf.weight_fmt = 1;  // 0 = unquantized weight matrix, 1 = qunatized
  conf.actfunc = 0;  // Activation Function: 0 = None, 1 = ReLU, 2 = Tanh, 3 = Leaky ReLU, 4 = Sigmoid, 5 = PReLU (PReLU must be used with POST-OP=1)
  conf.actfunc_param = 0x0;  // Leaky ReLU parameter (in FP16 format), 0 = non-leaky

  fpga_layer& layer = get_layer(4);
  layer.name = "dense_2";
  layer.type = LT_FC;
  layer.input_offs = 32;
  layer.output_offs = 16;
  layer.output_size = 4;
  layer.input_dim[0] = 1024;
  layer.input_dim_size = 1;
  layer.output_dim[0] = 2;
  layer.output_dim_size = 1;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = false;
}//end of  Layer_4

//Layer_5: SoftMax Layer
//	->: activation_4
void Cpumpkin20181218_verylight::Layer_5() {
  fpga_layer& layer = get_layer(5);
  layer.name = "activation_4";
  layer.type = LT_SOFTMAX;
  layer.input_offs = 16;
  layer.output_offs = 0;
  layer.output_size = 4;
  layer.input_dim[0] = 2;
  layer.input_dim_size = 1;
  layer.output_dim[0] = 2;
  layer.output_dim_size = 1;
  layer.is_output = true;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = false;
  layer.softmax_axis = 0;
  output_layers_[0] = &layer;
}//end of  Layer_5

