# Copyright 2021 Synamedia
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
objects          =							\
	SILABS_SUPERSET/Si_I2C/Silabs_L0_Connection.o			\
	SILABS_SUPERSET/Si2183/Si2183_L1_API.o				\
	SILABS_SUPERSET/Si2183/Si2183_L1_Commands.o			\
	SILABS_SUPERSET/Si2183/Si2183_L1_Properties.o			\
	SILABS_SUPERSET/Si2183/Si2183_L2_API.o				\
	SILABS_SUPERSET/Si2183/SiLabs_API_L3_Config_Macros.o		\
	SILABS_SUPERSET/Si2183/SiLabs_API_L3_Wrapper.o			\
	SILABS_SUPERSET/Si2183/SiLabs_API_L3_Wrapper_Channel_Bonding.o	\
	SILABS_SUPERSET/Si2183/SiLabs_API_L3_Wrapper_TS_Crossbar.o	\
	SILABS_SUPERSET/SAT/AV2018/SiLabs_L1_RF_AV2018_API.o		\
	SILABS_SUPERSET/SAT/SiLabs_SAT_Tuner_API.o			\
	SILABS_SUPERSET/TER/Si2141/Si2141_L1_Properties.o		\
	SILABS_SUPERSET/TER/Si2141/Si2141_L1_Commands.o			\
	SILABS_SUPERSET/TER/Si2141/Si2141_L1_API.o			\
	SILABS_SUPERSET/TER/Si2141/Si2141_L2_API.o			\
	SILABS_SUPERSET/TER/Si2141/Si2141_User_Properties.o		\
	SILABS_SUPERSET/TER/SiLabs_TER_Tuner_API.o
override CFLAGS :=							\
	-Wall								\
	-include config.h						\
	-I SILABS_SUPERSET/Si_I2C					\
	-I SILABS_SUPERSET/Si2183					\
	-I SILABS_SUPERSET/TER						\
	-I SILABS_SUPERSET/TER/Si2141					\
	-I SILABS_SUPERSET/SAT						\
	-I SILABS_SUPERSET/SAT/AV2018					\
	$(CFLAGS)
SILABS_SUPERSET/Si2183/SiLabs_API_L3_Console: $(objects)
clean:
	-rm -f $(objects)
