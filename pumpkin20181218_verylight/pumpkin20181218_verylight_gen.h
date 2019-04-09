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

class Cpumpkin20181218_verylight : public CDMP_Network {
 private:
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 0 | FPGA-Layer | Convolution | (64, 64, 1) | (32, 32, 32) | - | - |
  | 0-0 | conv2d_1 | Convolution | (64, 64, 1) | (64, 64, 32) | - | 2880 |
  | 0-0 | max_pooling2d_1 | Pooling | (64, 64, 32) | (32, 32, 32) | - | - |

  */
  void Layer_0();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 1 | FPGA-Layer | Convolution | (32, 32, 32) | (16, 16, 64) | - | - |
  | 1-0 | conv2d_2 | Convolution | (32, 32, 32) | (32, 32, 64) | - | 74368 |
  | 1-0 | max_pooling2d_2 | Pooling | (32, 32, 64) | (16, 16, 64) | - | - |

  */
  void Layer_1();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 2 | FPGA-Layer | Flatten | (16, 16, 64) | (16384,) | - | - |

  */
  void Layer_2();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 3 | FPGA-Layer | InnerProduct | (16384,) | (1024,) | - | - |

  */
  void Layer_3();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 4 | FPGA-Layer | InnerProduct | (1024,) | (2,) | - | - |

  */
  void Layer_4();
  /*!

  Layer description

  | ID | Layers | Type | Dim In | Dim Out | Param | Mem |
  | :- | :- | :-: | :-: | :-: | :-: | :-: |
  | 5 | FPGA-Layer | SoftMax | (2,) | (2,) | - | - |

  */
  void Layer_5();

 public:
  virtual bool Initialize();
  Cpumpkin20181218_verylight();
  virtual ~Cpumpkin20181218_verylight();
};
