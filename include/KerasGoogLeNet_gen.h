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
#pragma once

#include "dmp_network.h"

class CKerasGoogLeNet : public CDMP_Network {
 private:
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 0 | FPGA-Layer | Convolution | (224, 224, 3) | (56, 56, 64) | - | - |
  | 0-0 | conv1/7x7_s2 | Convolution | (224, 224, 3) | (112, 112, 64) | - | 14464 |
  | 0-0 | pool1/3x3_s2 | Pooling | (112, 112, 64) | (56, 56, 64) | - | - |

  */
  void Layer_0();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 1 | FPGA-Layer | Convolution | (56, 56, 64) | (56, 56, 64) | - | - |
  | 1-0 | pool1/norm1 | LRN | (56, 56, 64) | (56, 56, 64) | - | 0 |

  */
  void Layer_1();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 2 | FPGA-Layer | Convolution | (56, 56, 64) | (56, 56, 64) | - | - |
  | 2-0 | conv2/3x3_reduce | Convolution | (56, 56, 64) | (56, 56, 64) | - | 5248 |

  */
  void Layer_2();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 3 | FPGA-Layer | Convolution | (56, 56, 64) | (56, 56, 192) | - | - |
  | 3-0 | conv2/3x3 | Convolution | (56, 56, 64) | (56, 56, 192) | - | 111488 |

  */
  void Layer_3();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 4 | FPGA-Layer | Convolution | (56, 56, 192) | (56, 56, 192) | - | - |
  | 4-0 | conv2/norm2 | LRN | (56, 56, 192) | (56, 56, 192) | - | 0 |

  */
  void Layer_4();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 5 | FPGA-Layer | Convolution | (56, 56, 192) | (28, 28, 192) | - | - |
  | 5-0 | pool2/3x3_s2 | Pooling | (56, 56, 192) | (28, 28, 192) | - | - |

  */
  void Layer_5();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 6 | FPGA-Layer | Convolution | (28, 28, 192) | (28, 28, 256) | - | - |
  | 6-0 | inception_3a/1x1 | Convolution | (28, 28, 192) | (28, 28, 64) | - | 14464 |
  | 6-1 | inception_3a/3x3_reduce | Convolution | (28, 28, 192) | (28, 28, 96) | - | 21440 |
  | 6-2 | inception_3a/3x3 | Convolution | (28, 28, 96) | (28, 28, 128) | - | 111360 |
  | 6-3 | inception_3a/5x5_reduce | Convolution | (28, 28, 192) | (28, 28, 16) | - | 4000 |
  | 6-4 | inception_3a/5x5 | Convolution | (28, 28, 16) | (28, 28, 32) | - | 19008 |
  | 6-5 | inception_3a/pool | Pooling | (28, 28, 192) | (28, 28, 192) | - | - |
  | 6-6 | inception_3a/pool_proj | Convolution | (28, 28, 192) | (28, 28, 32) | - | 7488 |

  */
  void Layer_6();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 7 | FPGA-Layer | Convolution | (28, 28, 256) | (14, 14, 480) | - | - |
  | 7-0 | inception_3b/1x1 | Convolution | (28, 28, 256) | (28, 28, 128) | - | 37632 |
  | 7-0 | pool3/3x3_s2 | Pooling | (28, 28, 480) | (14, 14, 480) | - | - |
  | 7-1 | inception_3b/3x3_reduce | Convolution | (28, 28, 256) | (28, 28, 128) | - | 37632 |
  | 7-2 | inception_3b/3x3 | Convolution | (28, 28, 128) | (28, 28, 192) | - | 222080 |
  | 7-2 | pool3/3x3_s2 | Pooling | (28, 28, 480) | (14, 14, 480) | - | - |
  | 7-3 | inception_3b/5x5_reduce | Convolution | (28, 28, 256) | (28, 28, 32) | - | 9792 |
  | 7-4 | inception_3b/5x5 | Convolution | (28, 28, 32) | (28, 28, 96) | - | 111296 |
  | 7-4 | pool3/3x3_s2 | Pooling | (28, 28, 480) | (14, 14, 480) | - | - |
  | 7-5 | inception_3b/pool | Pooling | (28, 28, 256) | (28, 28, 256) | - | - |
  | 7-6 | inception_3b/pool_proj | Convolution | (28, 28, 256) | (28, 28, 64) | - | 19072 |
  | 7-6 | pool3/3x3_s2 | Pooling | (28, 28, 480) | (14, 14, 480) | - | - |

  */
  void Layer_7();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 8 | FPGA-Layer | Convolution | (14, 14, 480) | (14, 14, 512) | - | - |
  | 8-0 | inception_4a/1x1 | Convolution | (14, 14, 480) | (14, 14, 192) | - | 111488 |
  | 8-1 | inception_4a/3x3_reduce | Convolution | (14, 14, 480) | (14, 14, 96) | - | 56000 |
  | 8-2 | inception_4a/3x3 | Convolution | (14, 14, 96) | (14, 14, 208) | - | 180640 |
  | 8-3 | inception_4a/5x5_reduce | Convolution | (14, 14, 480) | (14, 14, 16) | - | 9760 |
  | 8-4 | inception_4a/5x5 | Convolution | (14, 14, 16) | (14, 14, 48) | - | 28256 |
  | 8-5 | inception_4a/pool | Pooling | (14, 14, 480) | (14, 14, 480) | - | - |
  | 8-6 | inception_4a/pool_proj | Convolution | (14, 14, 480) | (14, 14, 64) | - | 37504 |

  */
  void Layer_8();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 9 | FPGA-Layer | Convolution | (14, 14, 512) | (14, 14, 512) | - | - |
  | 9-0 | inception_4b/1x1 | Convolution | (14, 14, 512) | (14, 14, 160) | - | 92992 |
  | 9-1 | inception_4b/3x3_reduce | Convolution | (14, 14, 512) | (14, 14, 112) | - | 65248 |
  | 9-2 | inception_4b/3x3 | Convolution | (14, 14, 112) | (14, 14, 224) | - | 226752 |
  | 9-3 | inception_4b/5x5_reduce | Convolution | (14, 14, 512) | (14, 14, 24) | - | 14384 |
  | 9-4 | inception_4b/5x5 | Convolution | (14, 14, 24) | (14, 14, 64) | - | 55936 |
  | 9-5 | inception_4b/pool | Pooling | (14, 14, 512) | (14, 14, 512) | - | - |
  | 9-6 | inception_4b/pool_proj | Convolution | (14, 14, 512) | (14, 14, 64) | - | 37504 |

  */
  void Layer_9();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 10 | FPGA-Layer | Convolution | (14, 14, 512) | (14, 14, 512) | - | - |
  | 10-0 | inception_4c/1x1 | Convolution | (14, 14, 512) | (14, 14, 128) | - | 74496 |
  | 10-1 | inception_4c/3x3_reduce | Convolution | (14, 14, 512) | (14, 14, 128) | - | 74496 |
  | 10-2 | inception_4c/3x3 | Convolution | (14, 14, 128) | (14, 14, 256) | - | 295936 |
  | 10-3 | inception_4c/5x5_reduce | Convolution | (14, 14, 512) | (14, 14, 24) | - | 14384 |
  | 10-4 | inception_4c/5x5 | Convolution | (14, 14, 24) | (14, 14, 64) | - | 55936 |
  | 10-5 | inception_4c/pool | Pooling | (14, 14, 512) | (14, 14, 512) | - | - |
  | 10-6 | inception_4c/pool_proj | Convolution | (14, 14, 512) | (14, 14, 64) | - | 37504 |

  */
  void Layer_10();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 11 | FPGA-Layer | Convolution | (14, 14, 512) | (14, 14, 528) | - | - |
  | 11-0 | inception_4d/1x1 | Convolution | (14, 14, 512) | (14, 14, 112) | - | 65248 |
  | 11-1 | inception_4d/3x3_reduce | Convolution | (14, 14, 512) | (14, 14, 144) | - | 83744 |
  | 11-2 | inception_4d/3x3 | Convolution | (14, 14, 144) | (14, 14, 288) | - | 374336 |
  | 11-3 | inception_4d/5x5_reduce | Convolution | (14, 14, 512) | (14, 14, 32) | - | 19008 |
  | 11-4 | inception_4d/5x5 | Convolution | (14, 14, 32) | (14, 14, 64) | - | 74368 |
  | 11-5 | inception_4d/pool | Pooling | (14, 14, 512) | (14, 14, 512) | - | - |
  | 11-6 | inception_4d/pool_proj | Convolution | (14, 14, 512) | (14, 14, 64) | - | 37504 |

  */
  void Layer_11();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 12 | FPGA-Layer | Convolution | (14, 14, 528) | (7, 7, 832) | - | - |
  | 12-0 | inception_4e/1x1 | Convolution | (14, 14, 528) | (14, 14, 256) | - | 166912 |
  | 12-0 | pool4/3x3_s2 | Pooling | (14, 14, 832) | (7, 7, 832) | - | - |
  | 12-1 | inception_4e/3x3_reduce | Convolution | (14, 14, 528) | (14, 14, 160) | - | 104512 |
  | 12-2 | inception_4e/3x3 | Convolution | (14, 14, 160) | (14, 14, 320) | - | 461952 |
  | 12-2 | pool4/3x3_s2 | Pooling | (14, 14, 832) | (7, 7, 832) | - | - |
  | 12-3 | inception_4e/5x5_reduce | Convolution | (14, 14, 528) | (14, 14, 32) | - | 21312 |
  | 12-4 | inception_4e/5x5 | Convolution | (14, 14, 32) | (14, 14, 128) | - | 148224 |
  | 12-4 | pool4/3x3_s2 | Pooling | (14, 14, 832) | (7, 7, 832) | - | - |
  | 12-5 | inception_4e/pool | Pooling | (14, 14, 528) | (14, 14, 528) | - | - |
  | 12-6 | inception_4e/pool_proj | Convolution | (14, 14, 528) | (14, 14, 128) | - | 83712 |
  | 12-6 | pool4/3x3_s2 | Pooling | (14, 14, 832) | (7, 7, 832) | - | - |

  */
  void Layer_12();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 13 | FPGA-Layer | Convolution | (7, 7, 832) | (7, 7, 832) | - | - |
  | 13-0 | inception_5a/1x1 | Convolution | (7, 7, 832) | (7, 7, 256) | - | 240640 |
  | 13-1 | inception_5a/3x3_reduce | Convolution | (7, 7, 832) | (7, 7, 160) | - | 150592 |
  | 13-2 | inception_5a/3x3 | Convolution | (7, 7, 160) | (7, 7, 320) | - | 461952 |
  | 13-3 | inception_5a/5x5_reduce | Convolution | (7, 7, 832) | (7, 7, 32) | - | 30528 |
  | 13-4 | inception_5a/5x5 | Convolution | (7, 7, 32) | (7, 7, 128) | - | 148224 |
  | 13-5 | inception_5a/pool | Pooling | (7, 7, 832) | (7, 7, 832) | - | - |
  | 13-6 | inception_5a/pool_proj | Convolution | (7, 7, 832) | (7, 7, 128) | - | 120576 |

  */
  void Layer_13();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 14 | FPGA-Layer | Convolution | (7, 7, 832) | (7, 7, 1024) | - | - |
  | 14-0 | inception_5b/1x1 | Convolution | (7, 7, 832) | (7, 7, 384) | - | 360704 |
  | 14-1 | inception_5b/3x3_reduce | Convolution | (7, 7, 832) | (7, 7, 192) | - | 180608 |
  | 14-2 | inception_5b/3x3 | Convolution | (7, 7, 192) | (7, 7, 384) | - | 664832 |
  | 14-3 | inception_5b/5x5_reduce | Convolution | (7, 7, 832) | (7, 7, 48) | - | 45536 |
  | 14-4 | inception_5b/5x5 | Convolution | (7, 7, 48) | (7, 7, 128) | - | 221952 |
  | 14-5 | inception_5b/pool | Pooling | (7, 7, 832) | (7, 7, 832) | - | - |
  | 14-6 | inception_5b/pool_proj | Convolution | (7, 7, 832) | (7, 7, 128) | - | 120576 |

  */
  void Layer_14();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 15 | FPGA-Layer | Convolution | (7, 7, 1024) | (1024,) | - | - |
  | 15-0 | pool5/7x7_s2 | Pooling | (7, 7, 1024) | (1, 1, 1024) | - | - |

  */
  void Layer_15();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 16 | FPGA-Layer | InnerProduct | (1024,) | (1000,) | - | - |

  */
  void Layer_16();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 17 | FPGA-Layer | SoftMax | (1000,) | (1000,) | - | - |

  */
  void Layer_17();

 public:
  virtual bool Initialize();
  CKerasGoogLeNet();
  virtual ~CKerasGoogLeNet();
};
