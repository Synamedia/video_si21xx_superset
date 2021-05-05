/***********************************************************************
Copyright 2021 Synamedia

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 **********************************************************************/
/* Configuration. */
#define NO_WIN32              1
#define SILABS_EVB            1
#define CONFIG_MACROS         1
#define SILABS_EVB_MACROS     1
/* Demodulator. */
#define Si2183_B60_COMPATIBLE 1
#define FW_CHANGE_COMPATIBLE  1
#define DEMOD_Si2183          1
#define CHANNEL_BONDING       1
#define TS_CROSSBAR           1
/* Satellite. */
#define SATELLITE_FRONT_END   1
#define SAT_TUNER_SILABS      1
#define SAT_TUNER_AV2018      1
/* Terrestrial. */
#define TERRESTRIAL_FRONT_END 1
#define TER_TUNER_SILABS      1
#define TER_TUNER_Si2141      1
/* Testing. */
#define SILABS_API_TEST_PIPE  1
