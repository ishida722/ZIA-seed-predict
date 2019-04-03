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

#include "KerasGoogLeNet_gen.h"


CKerasGoogLeNet::CKerasGoogLeNet() {
  // Empty by design
}

CKerasGoogLeNet::~CKerasGoogLeNet() {
  // Empty by design
}

bool CKerasGoogLeNet::Initialize() {
  if (!ReserveMemory(7629744, 2809856)) {
    return false;
  }

  set_num_layers(18);
  set_num_output_layers(1);

  Layer_0();
  Layer_1();
  Layer_2();
  Layer_3();
  Layer_4();
  Layer_5();
  Layer_6();
  Layer_7();
  Layer_8();
  Layer_9();
  Layer_10();
  Layer_11();
  Layer_12();
  Layer_13();
  Layer_14();
  Layer_15();
  Layer_16();
  Layer_17();

  return true;
}

//Layer_0: Convolution Layer
//  ->: conv1/7x7_s2
//  ->: conv1/7x7_s2_relu
//  ->: pool1/3x3_s2
void CKerasGoogLeNet::Layer_0() {
  get_layer(0).name = "conv1/7x7_s2, conv1/7x7_s2_relu, pool1/3x3_s2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(0).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 224;  // Input Width
  conf.h = 224;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 3;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 301056;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv1/7x7_s2
  //->: conv1/7x7_s2_relu
  //->: pool1/3x3_s2
  conf.run[0].m = 64;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x7;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 0;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x3020302;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(0);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 301056;
  layer.output_size = 401408;
  layer.input_dim[0] = 224;
  layer.input_dim[1] = 224;
  layer.input_dim[2] = 3;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 56;
  layer.output_dim[1] = 56;
  layer.output_dim[2] = 64;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = false;
}//end of  Layer_0

//Layer_1: Convolution Layer
//  ->: pool1/norm1
void CKerasGoogLeNet::Layer_1() {
  get_layer(1).name = "pool1/norm1";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(1).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 56;  // Input Width
  conf.h = 56;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 64;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 301056;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 702464;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: pool1/norm1
  conf.run[0].m = 64;  // Output Channels
  conf.run[0].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 14464;
  conf.run[0].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x503;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(1);
  layer.type = LT_CONV;
  layer.input_offs = 301056;
  layer.output_offs = 702464;
  layer.output_size = 401408;
  layer.input_dim[0] = 56;
  layer.input_dim[1] = 56;
  layer.input_dim[2] = 64;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 56;
  layer.output_dim[1] = 56;
  layer.output_dim[2] = 64;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_1

//Layer_2: Convolution Layer
//  ->: conv2/3x3_reduce
//  ->: conv2/3x3_reduce_relu
void CKerasGoogLeNet::Layer_2() {
  get_layer(2).name = "conv2/3x3_reduce, conv2/3x3_reduce_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(2).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 56;  // Input Width
  conf.h = 56;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 64;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 702464;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv2/3x3_reduce
  //->: conv2/3x3_reduce_relu
  conf.run[0].m = 64;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 14464;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(2);
  layer.type = LT_CONV;
  layer.input_offs = 702464;
  layer.output_offs = 0;
  layer.output_size = 401408;
  layer.input_dim[0] = 56;
  layer.input_dim[1] = 56;
  layer.input_dim[2] = 64;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 56;
  layer.output_dim[1] = 56;
  layer.output_dim[2] = 64;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_2

//Layer_3: Convolution Layer
//  ->: conv2/3x3
//  ->: conv2/3x3_relu
void CKerasGoogLeNet::Layer_3() {
  get_layer(3).name = "conv2/3x3, conv2/3x3_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(3).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 56;  // Input Width
  conf.h = 56;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 64;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 401408;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv2/3x3
  //->: conv2/3x3_relu
  conf.run[0].m = 192;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x3;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 19712;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(3);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 401408;
  layer.output_size = 1204224;
  layer.input_dim[0] = 56;
  layer.input_dim[1] = 56;
  layer.input_dim[2] = 64;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 56;
  layer.output_dim[1] = 56;
  layer.output_dim[2] = 192;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_3

//Layer_4: Convolution Layer
//  ->: conv2/norm2
void CKerasGoogLeNet::Layer_4() {
  get_layer(4).name = "conv2/norm2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(4).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 56;  // Input Width
  conf.h = 56;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 192;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 401408;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 1605632;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: conv2/norm2
  conf.run[0].m = 192;  // Output Channels
  conf.run[0].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 131200;
  conf.run[0].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x503;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(4);
  layer.type = LT_CONV;
  layer.input_offs = 401408;
  layer.output_offs = 1605632;
  layer.output_size = 1204224;
  layer.input_dim[0] = 56;
  layer.input_dim[1] = 56;
  layer.input_dim[2] = 192;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 56;
  layer.output_dim[1] = 56;
  layer.output_dim[2] = 192;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_4

//Layer_5: Convolution Layer
//  ->: pool2/3x3_s2
void CKerasGoogLeNet::Layer_5() {
  get_layer(5).name = "pool2/3x3_s2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(5).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 56;  // Input Width
  conf.h = 56;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 192;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 1605632;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: pool2/3x3_s2
  conf.run[0].m = 192;  // Output Channels
  conf.run[0].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 131200;
  conf.run[0].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(5);
  layer.type = LT_CONV;
  layer.input_offs = 1605632;
  layer.output_offs = 0;
  layer.output_size = 301056;
  layer.input_dim[0] = 56;
  layer.input_dim[1] = 56;
  layer.input_dim[2] = 192;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 28;
  layer.output_dim[1] = 28;
  layer.output_dim[2] = 192;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_5

//Layer_6: Convolution Layer
//  ->: inception_3a/1x1
//  ->: inception_3a/1x1_relu
//  ->: inception_3a/3x3_reduce
//  ->: inception_3a/3x3_reduce_relu
//  ->: inception_3a/3x3
//  ->: inception_3a/3x3_relu
//  ->: inception_3a/5x5_reduce
//  ->: inception_3a/5x5_reduce_relu
//  ->: inception_3a/5x5
//  ->: inception_3a/5x5_relu
//  ->: inception_3a/pool
//  ->: inception_3a/pool_proj
//  ->: inception_3a/pool_proj_relu
void CKerasGoogLeNet::Layer_6() {
  get_layer(6).name = "inception_3a/1x1, inception_3a/1x1_relu, inception_3a/3x3_reduce, inception_3a/3x3_reduce_relu, inception_3a/3x3, inception_3a/3x3_relu, inception_3a/5x5_reduce, inception_3a/5x5_reduce_relu, inception_3a/5x5, inception_3a/5x5_relu, inception_3a/pool, inception_3a/pool_proj, inception_3a/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(6).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 28;  // Input Width
  conf.h = 28;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 192;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 301056;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_3a/1x1
  //->: inception_3a/1x1_relu
  conf.run[0].m = 64;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 131200;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_3a/3x3_reduce
  //->: inception_3a/3x3_reduce_relu
  conf.run[1].m = 96;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 145664;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_3a/3x3
  //->: inception_3a/3x3_relu
  conf.run[2].m = 128;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 167104;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_3a/5x5_reduce
  //->: inception_3a/5x5_reduce_relu
  conf.run[3].m = 16;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 278464;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_3a/5x5
  //->: inception_3a/5x5_relu
  conf.run[4].m = 32;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 282464;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_3a/pool
  conf.run[5].m = 192;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 301472;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_3a/pool_proj
  //->: inception_3a/pool_proj_relu
  conf.run[6].m = 32;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 301472;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(6);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 301056;
  layer.output_size = 401408;
  layer.input_dim[0] = 28;
  layer.input_dim[1] = 28;
  layer.input_dim[2] = 192;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 28;
  layer.output_dim[1] = 28;
  layer.output_dim[2] = 256;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_6

//Layer_7: Convolution Layer
//  ->: inception_3b/1x1
//  ->: inception_3b/1x1_relu
//  ->: pool3/3x3_s2
//  ->: inception_3b/3x3_reduce
//  ->: inception_3b/3x3_reduce_relu
//  ->: inception_3b/3x3
//  ->: inception_3b/3x3_relu
//  ->: pool3/3x3_s2
//  ->: inception_3b/5x5_reduce
//  ->: inception_3b/5x5_reduce_relu
//  ->: inception_3b/5x5
//  ->: inception_3b/5x5_relu
//  ->: pool3/3x3_s2
//  ->: inception_3b/pool
//  ->: inception_3b/pool_proj
//  ->: inception_3b/pool_proj_relu
//  ->: pool3/3x3_s2
void CKerasGoogLeNet::Layer_7() {
  get_layer(7).name = "inception_3b/1x1, inception_3b/1x1_relu, pool3/3x3_s2, inception_3b/3x3_reduce, inception_3b/3x3_reduce_relu, inception_3b/3x3, inception_3b/3x3_relu, pool3/3x3_s2, inception_3b/5x5_reduce, inception_3b/5x5_reduce_relu, inception_3b/5x5, inception_3b/5x5_relu, pool3/3x3_s2, inception_3b/pool, inception_3b/pool_proj, inception_3b/pool_proj_relu, pool3/3x3_s2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(7).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 28;  // Input Width
  conf.h = 28;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 256;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 301056;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_3b/1x1
  //->: inception_3b/1x1_relu
  //->: pool3/3x3_s2
  conf.run[0].m = 128;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 308960;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_3b/3x3_reduce
  //->: inception_3b/3x3_reduce_relu
  conf.run[1].m = 128;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 346592;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_3b/3x3
  //->: inception_3b/3x3_relu
  //->: pool3/3x3_s2
  conf.run[2].m = 192;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 384224;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_3b/5x5_reduce
  //->: inception_3b/5x5_reduce_relu
  conf.run[3].m = 32;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 606304;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_3b/5x5
  //->: inception_3b/5x5_relu
  //->: pool3/3x3_s2
  conf.run[4].m = 96;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 616096;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_3b/pool
  conf.run[5].m = 256;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 727392;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_3b/pool_proj
  //->: inception_3b/pool_proj_relu
  //->: pool3/3x3_s2
  conf.run[6].m = 64;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 727392;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(7);
  layer.type = LT_CONV;
  layer.input_offs = 301056;
  layer.output_offs = 0;
  layer.output_size = 188160;
  layer.input_dim[0] = 28;
  layer.input_dim[1] = 28;
  layer.input_dim[2] = 256;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 14;
  layer.output_dim[1] = 14;
  layer.output_dim[2] = 480;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_7

//Layer_8: Convolution Layer
//  ->: inception_4a/1x1
//  ->: inception_4a/1x1_relu
//  ->: inception_4a/3x3_reduce
//  ->: inception_4a/3x3_reduce_relu
//  ->: inception_4a/3x3
//  ->: inception_4a/3x3_relu
//  ->: inception_4a/5x5_reduce
//  ->: inception_4a/5x5_reduce_relu
//  ->: inception_4a/5x5
//  ->: inception_4a/5x5_relu
//  ->: inception_4a/pool
//  ->: inception_4a/pool_proj
//  ->: inception_4a/pool_proj_relu
void CKerasGoogLeNet::Layer_8() {
  get_layer(8).name = "inception_4a/1x1, inception_4a/1x1_relu, inception_4a/3x3_reduce, inception_4a/3x3_reduce_relu, inception_4a/3x3, inception_4a/3x3_relu, inception_4a/5x5_reduce, inception_4a/5x5_reduce_relu, inception_4a/5x5, inception_4a/5x5_relu, inception_4a/pool, inception_4a/pool_proj, inception_4a/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(8).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 14;  // Input Width
  conf.h = 14;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 480;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 188160;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_4a/1x1
  //->: inception_4a/1x1_relu
  conf.run[0].m = 192;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 746464;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_4a/3x3_reduce
  //->: inception_4a/3x3_reduce_relu
  conf.run[1].m = 96;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 857952;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_4a/3x3
  //->: inception_4a/3x3_relu
  conf.run[2].m = 208;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 913952;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_4a/5x5_reduce
  //->: inception_4a/5x5_reduce_relu
  conf.run[3].m = 16;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 1094592;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_4a/5x5
  //->: inception_4a/5x5_relu
  conf.run[4].m = 48;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 1104352;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_4a/pool
  conf.run[5].m = 480;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 1132608;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_4a/pool_proj
  //->: inception_4a/pool_proj_relu
  conf.run[6].m = 64;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 1132608;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(8);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 188160;
  layer.output_size = 200704;
  layer.input_dim[0] = 14;
  layer.input_dim[1] = 14;
  layer.input_dim[2] = 480;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 14;
  layer.output_dim[1] = 14;
  layer.output_dim[2] = 512;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_8

//Layer_9: Convolution Layer
//  ->: inception_4b/1x1
//  ->: inception_4b/1x1_relu
//  ->: inception_4b/3x3_reduce
//  ->: inception_4b/3x3_reduce_relu
//  ->: inception_4b/3x3
//  ->: inception_4b/3x3_relu
//  ->: inception_4b/5x5_reduce
//  ->: inception_4b/5x5_reduce_relu
//  ->: inception_4b/5x5
//  ->: inception_4b/5x5_relu
//  ->: inception_4b/pool
//  ->: inception_4b/pool_proj
//  ->: inception_4b/pool_proj_relu
void CKerasGoogLeNet::Layer_9() {
  get_layer(9).name = "inception_4b/1x1, inception_4b/1x1_relu, inception_4b/3x3_reduce, inception_4b/3x3_reduce_relu, inception_4b/3x3, inception_4b/3x3_relu, inception_4b/5x5_reduce, inception_4b/5x5_reduce_relu, inception_4b/5x5, inception_4b/5x5_relu, inception_4b/pool, inception_4b/pool_proj, inception_4b/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(9).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 14;  // Input Width
  conf.h = 14;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 512;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 188160;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 388864;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_4b/1x1
  //->: inception_4b/1x1_relu
  conf.run[0].m = 160;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 1170112;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_4b/3x3_reduce
  //->: inception_4b/3x3_reduce_relu
  conf.run[1].m = 112;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 1263104;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_4b/3x3
  //->: inception_4b/3x3_relu
  conf.run[2].m = 224;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 1328352;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_4b/5x5_reduce
  //->: inception_4b/5x5_reduce_relu
  conf.run[3].m = 24;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 1555104;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_4b/5x5
  //->: inception_4b/5x5_relu
  conf.run[4].m = 64;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 1569488;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_4b/pool
  conf.run[5].m = 512;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 1625424;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_4b/pool_proj
  //->: inception_4b/pool_proj_relu
  conf.run[6].m = 64;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 1625424;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(9);
  layer.type = LT_CONV;
  layer.input_offs = 188160;
  layer.output_offs = 388864;
  layer.output_size = 200704;
  layer.input_dim[0] = 14;
  layer.input_dim[1] = 14;
  layer.input_dim[2] = 512;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 14;
  layer.output_dim[1] = 14;
  layer.output_dim[2] = 512;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_9

//Layer_10: Convolution Layer
//  ->: inception_4c/1x1
//  ->: inception_4c/1x1_relu
//  ->: inception_4c/3x3_reduce
//  ->: inception_4c/3x3_reduce_relu
//  ->: inception_4c/3x3
//  ->: inception_4c/3x3_relu
//  ->: inception_4c/5x5_reduce
//  ->: inception_4c/5x5_reduce_relu
//  ->: inception_4c/5x5
//  ->: inception_4c/5x5_relu
//  ->: inception_4c/pool
//  ->: inception_4c/pool_proj
//  ->: inception_4c/pool_proj_relu
void CKerasGoogLeNet::Layer_10() {
  get_layer(10).name = "inception_4c/1x1, inception_4c/1x1_relu, inception_4c/3x3_reduce, inception_4c/3x3_reduce_relu, inception_4c/3x3, inception_4c/3x3_relu, inception_4c/5x5_reduce, inception_4c/5x5_reduce_relu, inception_4c/5x5, inception_4c/5x5_relu, inception_4c/pool, inception_4c/pool_proj, inception_4c/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(10).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 14;  // Input Width
  conf.h = 14;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 512;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 388864;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_4c/1x1
  //->: inception_4c/1x1_relu
  conf.run[0].m = 128;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 1662928;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_4c/3x3_reduce
  //->: inception_4c/3x3_reduce_relu
  conf.run[1].m = 128;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 1737424;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_4c/3x3
  //->: inception_4c/3x3_relu
  conf.run[2].m = 256;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 1811920;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_4c/5x5_reduce
  //->: inception_4c/5x5_reduce_relu
  conf.run[3].m = 24;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 2107856;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_4c/5x5
  //->: inception_4c/5x5_relu
  conf.run[4].m = 64;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 2122240;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_4c/pool
  conf.run[5].m = 512;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 2178176;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_4c/pool_proj
  //->: inception_4c/pool_proj_relu
  conf.run[6].m = 64;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 2178176;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(10);
  layer.type = LT_CONV;
  layer.input_offs = 388864;
  layer.output_offs = 0;
  layer.output_size = 200704;
  layer.input_dim[0] = 14;
  layer.input_dim[1] = 14;
  layer.input_dim[2] = 512;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 14;
  layer.output_dim[1] = 14;
  layer.output_dim[2] = 512;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_10

//Layer_11: Convolution Layer
//  ->: inception_4d/1x1
//  ->: inception_4d/1x1_relu
//  ->: inception_4d/3x3_reduce
//  ->: inception_4d/3x3_reduce_relu
//  ->: inception_4d/3x3
//  ->: inception_4d/3x3_relu
//  ->: inception_4d/5x5_reduce
//  ->: inception_4d/5x5_reduce_relu
//  ->: inception_4d/5x5
//  ->: inception_4d/5x5_relu
//  ->: inception_4d/pool
//  ->: inception_4d/pool_proj
//  ->: inception_4d/pool_proj_relu
void CKerasGoogLeNet::Layer_11() {
  get_layer(11).name = "inception_4d/1x1, inception_4d/1x1_relu, inception_4d/3x3_reduce, inception_4d/3x3_reduce_relu, inception_4d/3x3, inception_4d/3x3_relu, inception_4d/5x5_reduce, inception_4d/5x5_reduce_relu, inception_4d/5x5, inception_4d/5x5_relu, inception_4d/pool, inception_4d/pool_proj, inception_4d/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(11).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 14;  // Input Width
  conf.h = 14;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 512;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 200704;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_4d/1x1
  //->: inception_4d/1x1_relu
  conf.run[0].m = 112;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 2215680;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_4d/3x3_reduce
  //->: inception_4d/3x3_reduce_relu
  conf.run[1].m = 144;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 2280928;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_4d/3x3
  //->: inception_4d/3x3_relu
  conf.run[2].m = 288;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 2364672;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_4d/5x5_reduce
  //->: inception_4d/5x5_reduce_relu
  conf.run[3].m = 32;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 2739008;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_4d/5x5
  //->: inception_4d/5x5_relu
  conf.run[4].m = 64;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 2758016;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_4d/pool
  conf.run[5].m = 512;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 2832384;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_4d/pool_proj
  //->: inception_4d/pool_proj_relu
  conf.run[6].m = 64;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 2832384;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(11);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 200704;
  layer.output_size = 206976;
  layer.input_dim[0] = 14;
  layer.input_dim[1] = 14;
  layer.input_dim[2] = 512;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 14;
  layer.output_dim[1] = 14;
  layer.output_dim[2] = 528;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_11

//Layer_12: Convolution Layer
//  ->: inception_4e/1x1
//  ->: inception_4e/1x1_relu
//  ->: pool4/3x3_s2
//  ->: inception_4e/3x3_reduce
//  ->: inception_4e/3x3_reduce_relu
//  ->: inception_4e/3x3
//  ->: inception_4e/3x3_relu
//  ->: pool4/3x3_s2
//  ->: inception_4e/5x5_reduce
//  ->: inception_4e/5x5_reduce_relu
//  ->: inception_4e/5x5
//  ->: inception_4e/5x5_relu
//  ->: pool4/3x3_s2
//  ->: inception_4e/pool
//  ->: inception_4e/pool_proj
//  ->: inception_4e/pool_proj_relu
//  ->: pool4/3x3_s2
void CKerasGoogLeNet::Layer_12() {
  get_layer(12).name = "inception_4e/1x1, inception_4e/1x1_relu, pool4/3x3_s2, inception_4e/3x3_reduce, inception_4e/3x3_reduce_relu, inception_4e/3x3, inception_4e/3x3_relu, pool4/3x3_s2, inception_4e/5x5_reduce, inception_4e/5x5_reduce_relu, inception_4e/5x5, inception_4e/5x5_relu, pool4/3x3_s2, inception_4e/pool, inception_4e/pool_proj, inception_4e/pool_proj_relu, pool4/3x3_s2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(12).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 14;  // Input Width
  conf.h = 14;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 528;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 200704;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_4e/1x1
  //->: inception_4e/1x1_relu
  //->: pool4/3x3_s2
  conf.run[0].m = 256;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 2869888;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_4e/3x3_reduce
  //->: inception_4e/3x3_reduce_relu
  conf.run[1].m = 160;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 3036800;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_4e/3x3
  //->: inception_4e/3x3_relu
  //->: pool4/3x3_s2
  conf.run[2].m = 320;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 3141312;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_4e/5x5_reduce
  //->: inception_4e/5x5_reduce_relu
  conf.run[3].m = 32;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 3603264;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_4e/5x5
  //->: inception_4e/5x5_relu
  //->: pool4/3x3_s2
  conf.run[4].m = 128;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 3624576;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_4e/pool
  conf.run[5].m = 528;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 3772800;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_4e/pool_proj
  //->: inception_4e/pool_proj_relu
  //->: pool4/3x3_s2
  conf.run[6].m = 128;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 3772800;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x202;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x1000100;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(12);
  layer.type = LT_CONV;
  layer.input_offs = 200704;
  layer.output_offs = 0;
  layer.output_size = 81536;
  layer.input_dim[0] = 14;
  layer.input_dim[1] = 14;
  layer.input_dim[2] = 528;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 7;
  layer.output_dim[1] = 7;
  layer.output_dim[2] = 832;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_12

//Layer_13: Convolution Layer
//  ->: inception_5a/1x1
//  ->: inception_5a/1x1_relu
//  ->: inception_5a/3x3_reduce
//  ->: inception_5a/3x3_reduce_relu
//  ->: inception_5a/3x3
//  ->: inception_5a/3x3_relu
//  ->: inception_5a/5x5_reduce
//  ->: inception_5a/5x5_reduce_relu
//  ->: inception_5a/5x5
//  ->: inception_5a/5x5_relu
//  ->: inception_5a/pool
//  ->: inception_5a/pool_proj
//  ->: inception_5a/pool_proj_relu
void CKerasGoogLeNet::Layer_13() {
  get_layer(13).name = "inception_5a/1x1, inception_5a/1x1_relu, inception_5a/3x3_reduce, inception_5a/3x3_reduce_relu, inception_5a/3x3, inception_5a/3x3_relu, inception_5a/5x5_reduce, inception_5a/5x5_reduce_relu, inception_5a/5x5, inception_5a/5x5_relu, inception_5a/pool, inception_5a/pool_proj, inception_5a/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(13).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 7;  // Input Width
  conf.h = 7;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 832;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 81536;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_5a/1x1
  //->: inception_5a/1x1_relu
  conf.run[0].m = 256;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 3856512;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_5a/3x3_reduce
  //->: inception_5a/3x3_reduce_relu
  conf.run[1].m = 160;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 4097152;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_5a/3x3
  //->: inception_5a/3x3_relu
  conf.run[2].m = 320;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 4247744;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_5a/5x5_reduce
  //->: inception_5a/5x5_reduce_relu
  conf.run[3].m = 32;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 4709696;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_5a/5x5
  //->: inception_5a/5x5_relu
  conf.run[4].m = 128;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 4740224;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_5a/pool
  conf.run[5].m = 832;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 4888448;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_5a/pool_proj
  //->: inception_5a/pool_proj_relu
  conf.run[6].m = 128;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 4888448;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(13);
  layer.type = LT_CONV;
  layer.input_offs = 0;
  layer.output_offs = 81536;
  layer.output_size = 81536;
  layer.input_dim[0] = 7;
  layer.input_dim[1] = 7;
  layer.input_dim[2] = 832;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 7;
  layer.output_dim[1] = 7;
  layer.output_dim[2] = 832;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_13

//Layer_14: Convolution Layer
//  ->: inception_5b/1x1
//  ->: inception_5b/1x1_relu
//  ->: inception_5b/3x3_reduce
//  ->: inception_5b/3x3_reduce_relu
//  ->: inception_5b/3x3
//  ->: inception_5b/3x3_relu
//  ->: inception_5b/5x5_reduce
//  ->: inception_5b/5x5_reduce_relu
//  ->: inception_5b/5x5
//  ->: inception_5b/5x5_relu
//  ->: inception_5b/pool
//  ->: inception_5b/pool_proj
//  ->: inception_5b/pool_proj_relu
void CKerasGoogLeNet::Layer_14() {
  get_layer(14).name = "inception_5b/1x1, inception_5b/1x1_relu, inception_5b/3x3_reduce, inception_5b/3x3_reduce_relu, inception_5b/3x3, inception_5b/3x3_relu, inception_5b/5x5_reduce, inception_5b/5x5_reduce_relu, inception_5b/5x5, inception_5b/5x5_relu, inception_5b/pool, inception_5b/pool_proj, inception_5b/pool_proj_relu";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(14).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000001010101
  conf.topo = 0x55;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 7;  // Input Width
  conf.h = 7;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 832;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 81536;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 163072;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->7 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: inception_5b/1x1
  //->: inception_5b/1x1_relu
  conf.run[0].m = 384;  // Output Channels
  conf.run[0].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 5009024;
  conf.run[0].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 1
  //--------------------------------------------------
  //->: inception_5b/3x3_reduce
  //->: inception_5b/3x3_reduce_relu
  conf.run[1].m = 192;  // Output Channels
  conf.run[1].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[1].p = 0x1;  // Filter Width and Height
  conf.run[1].pz = 1;  // Filter Depth
  conf.run[1].weight_buf.mem = weights_mem_;
  conf.run[1].weight_buf.offs = 5369728;
  conf.run[1].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[1].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[1].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[1].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[1].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[1].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[1].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[1].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[1].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[1].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[1].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 2
  //--------------------------------------------------
  //->: inception_5b/3x3
  //->: inception_5b/3x3_relu
  conf.run[2].m = 384;  // Output Channels
  conf.run[2].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[2].p = 0x3;  // Filter Width and Height
  conf.run[2].pz = 1;  // Filter Depth
  conf.run[2].weight_buf.mem = weights_mem_;
  conf.run[2].weight_buf.offs = 5550336;
  conf.run[2].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[2].conv_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[2].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[2].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[2].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[2].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[2].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[2].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[2].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[2].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[2].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 3
  //--------------------------------------------------
  //->: inception_5b/5x5_reduce
  //->: inception_5b/5x5_reduce_relu
  conf.run[3].m = 48;  // Output Channels
  conf.run[3].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[3].p = 0x1;  // Filter Width and Height
  conf.run[3].pz = 1;  // Filter Depth
  conf.run[3].weight_buf.mem = weights_mem_;
  conf.run[3].weight_buf.offs = 6215168;
  conf.run[3].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[3].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[3].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[3].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[3].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[3].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[3].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[3].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[3].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[3].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[3].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 4
  //--------------------------------------------------
  //->: inception_5b/5x5
  //->: inception_5b/5x5_relu
  conf.run[4].m = 128;  // Output Channels
  conf.run[4].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[4].p = 0x5;  // Filter Width and Height
  conf.run[4].pz = 1;  // Filter Depth
  conf.run[4].weight_buf.mem = weights_mem_;
  conf.run[4].weight_buf.offs = 6260704;
  conf.run[4].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[4].conv_pad = 0x2020202;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[4].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[4].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[4].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[4].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[4].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[4].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[4].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[4].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[4].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 5
  //--------------------------------------------------
  //->: inception_5b/pool
  conf.run[5].m = 832;  // Output Channels
  conf.run[5].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[5].p = 0x1;  // Filter Width and Height
  conf.run[5].pz = 1;  // Filter Depth
  conf.run[5].weight_buf.mem = weights_mem_;
  conf.run[5].weight_buf.offs = 6482656;
  conf.run[5].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[5].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[5].pool_enable = 1;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[5].pool_size = 0x303;  // bits [7:0] = width, bits [15:8] = height
  conf.run[5].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[5].pool_pad = 0x1010101;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[5].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[5].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[5].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[5].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[5].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2
  //--------------------------------------------------
  //RUN : 6
  //--------------------------------------------------
  //->: inception_5b/pool_proj
  //->: inception_5b/pool_proj_relu
  conf.run[6].m = 128;  // Output Channels
  conf.run[6].conv_enable = 1;  // 1 = Enabled, 0 = Disabled
  conf.run[6].p = 0x1;  // Filter Width and Height
  conf.run[6].pz = 1;  // Filter Depth
  conf.run[6].weight_buf.mem = weights_mem_;
  conf.run[6].weight_buf.offs = 6482656;
  conf.run[6].weight_fmt = 3;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[6].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[6].pool_enable = 0;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[6].pool_size = 0x0;  // bits [7:0] = width, bits [15:8] = height
  conf.run[6].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[6].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[6].pool_avg_param = 0x0;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[6].actfunc = 2;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[6].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[6].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[6].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(14);
  layer.type = LT_CONV;
  layer.input_offs = 81536;
  layer.output_offs = 163072;
  layer.output_size = 100352;
  layer.input_dim[0] = 7;
  layer.input_dim[1] = 7;
  layer.input_dim[2] = 832;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 7;
  layer.output_dim[1] = 7;
  layer.output_dim[2] = 1024;
  layer.output_dim_size = 3;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_14

//Layer_15: Convolution Layer
//  ->: pool5/7x7_s2
void CKerasGoogLeNet::Layer_15() {
  get_layer(15).name = "pool5/7x7_s2";
  dmp_dv_cmdraw_conv_v0& conf = get_layer(15).conv_conf;
  conf.header.size = sizeof(conf);
  conf.header.device_type = DMP_DV_DEV_CONV;
  conf.header.version = 0;
  // Topo: 00000000000000000000000000000001
  conf.topo = 0x1;  // [31:0] Output Destination of each run, 0 = UBUF, 1 = EXTMEM

  // Input Configuration:
  conf.w = 7;  // Input Width
  conf.h = 7;  // Input Height
  conf.z = 1;  // Input Depth
  conf.c = 1024;  // Input Channels
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 163072;

  // Output Configuration:
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 0;

  conf.eltwise_buf.mem = NULL;
  conf.eltwise_buf.offs = 0;  // Input byte address for elementwise add (0 = UBUF Input Buffer)
  conf.output_mode = 0;  // 0 = concat, 1 = eltwise add

  // Runs Configuration:
  // ->1 run(s)
  //--------------------------------------------------
  //RUN : 0
  //--------------------------------------------------
  //->: pool5/7x7_s2
  conf.run[0].m = 1024;  // Output Channels
  conf.run[0].conv_enable = 0;  // 1 = Enabled, 0 = Disabled
  conf.run[0].p = 0x1;  // Filter Width and Height
  conf.run[0].pz = 1;  // Filter Depth
  conf.run[0].weight_buf.mem = weights_mem_;
  conf.run[0].weight_buf.offs = 6603232;
  conf.run[0].weight_fmt = 0;  // Weight format (0 = random access blocks, 1 = compact stream, 3 = 8-bit qunatized stream)
  conf.run[0].conv_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].conv_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].conv_dilation = 0x0;  // bits [7:0] = X dilation, bits [15:8] = Y dilation
  conf.run[0].pool_enable = 2;  // 0 = disabled, 1 = max pooling, 2 = average pooling
  conf.run[0].pool_size = 0x707;  // bits [7:0] = width, bits [15:8] = height
  conf.run[0].pool_stride = 0x101;  // bits [7:0] = X stride, bits [15:8] = Y stride
  conf.run[0].pool_pad = 0x0;  // bits [7:0] = left padding, bits [15:8] = right padding, bits [23:16] = top padding, bits [31:24] = bottom padding
  conf.run[0].pool_avg_param = 0x2539;  // Usually set to 1/pool_size^2 in FP16 format when using average pooling (average pooling assumes square size)
  conf.run[0].actfunc = 0;  // Activation Function: 0 = None, 1 = Tanh, 2 = Leaky ReLU, 3 = Sigmoid, 4 = PReLU, 5 = ELU, 6 = ReLU6
  conf.run[0].actfunc_param = 0x0;  // Leaky ReLU parameter (NOTE: 0x2E66 is 0.1 in FP16)
  conf.run[0].rectifi_en = 0;  // Rectification, i.e. max(0, x) (NOTE: Can be applied after non-ReLU activation function)
  conf.run[0].lrn = 0x0;  // [0] : 1 = LRN enable, 0 = LRN disable, [1] : 1 = incl. power func, 0 = excl., [8:11] = x^2 scale factor log2

  fpga_layer& layer = get_layer(15);
  layer.type = LT_CONV;
  layer.input_offs = 163072;
  layer.output_offs = 0;
  layer.output_size = 2048;
  layer.input_dim[0] = 7;
  layer.input_dim[1] = 7;
  layer.input_dim[2] = 1024;
  layer.input_dim_size = 3;
  layer.output_dim[0] = 1024;
  layer.output_dim_size = 1;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_15

// Layer_16: Fully Connected Layer
//	->: loss3/classifier
void CKerasGoogLeNet::Layer_16() {
  get_layer(16).name = "loss3/classifier";
  dmp_dv_cmdraw_fc_v0& conf = get_layer(16).fc_conf;
  conf.header.size = sizeof(conf);
  conf.header.version = 0;
  conf.header.device_type = DMP_DV_DEV_FC;
  conf.input_size = 1024;
  conf.output_size = 1000;
  conf.weight_buf.mem = weights_mem_;
  conf.weight_buf.offs = 6603232;
  conf.input_buf.mem = io_mem_;
  conf.input_buf.offs = 0;
  conf.output_buf.mem = io_mem_;
  conf.output_buf.offs = 2048;
  conf.weight_fmt = 1;  // 0 = unquantized weight matrix, 1 = qunatized
  conf.actfunc = 0;  // Activation Function: 0 = None, 1 = ReLU, 2 = Tanh, 3 = Leaky ReLU, 4 = Sigmoid, 5 = PReLU (PReLU must be used with POST-OP=1)
  conf.actfunc_param = 0x0;  // Leaky ReLU parameter (in FP16 format), 0 = non-leaky

  fpga_layer& layer = get_layer(16);
  layer.type = LT_FC;
  layer.input_offs = 0;
  layer.output_offs = 2048;
  layer.output_size = 2000;
  layer.input_dim[0] = 1024;
  layer.input_dim_size = 1;
  layer.output_dim[0] = 1000;
  layer.output_dim_size = 1;
  layer.is_output = false;
  layer.is_f32_output = false;
  layer.is_input_hw_layout = true;
}//end of  Layer_16

//Layer_17: SoftMax Layer
//	->: prob
void CKerasGoogLeNet::Layer_17() {
  fpga_layer& layer = get_layer(17);
  layer.type = LT_SOFTMAX;
  layer.input_offs = 2048;
  layer.output_offs = 4048;
  layer.output_size = 4000;
  layer.input_dim[0] = 1000;
  layer.input_dim_size = 1;
  layer.output_dim[0] = 1000;
  layer.output_dim_size = 1;
  layer.is_output = true;
  layer.is_f32_output = true;
  layer.is_input_hw_layout = false;
  layer.softmax_axis = 0;
  output_layers_[0] = &layer;
}//end of  Layer_17

