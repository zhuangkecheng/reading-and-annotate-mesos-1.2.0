// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stout/none.hpp>
#include <stout/subcommand.hpp>

#include <stout/os/socket.hpp>

#include "slave/containerizer/mesos/launch.hpp"
#include "slave/containerizer/mesos/mount.hpp"

#ifdef __linux__
#include "slave/containerizer/mesos/isolators/network/cni/cni.hpp"
#endif

using namespace mesos::internal::slave;

//mesos-slave的src/slave main.c函数   mesos-containerizer的main函数在src/slave/containerizer/mesos/main.cpp
int main(int argc, char** argv)
{
#ifdef __WINDOWS__
  // Initialize the Windows socket stack.
  if (!net::wsa_initialize()) {
    EXIT(EXIT_FAILURE) << "Failed to initialize the WSA socket stack";
  }
#endif // __WINDOWS__

  /*
  [root@localhost src]# ./mesos-containerizer launch
  yang test ............... main
  WARNING: Logging before InitGoogleLogging() is written to STDERR
  I0810 17:28:00.322547 16996 launch.cpp:243] MesosContainerizerLaunch::execute
  I0810 17:28:00.322774 16996 launch.cpp:171] installSignalHandlers
  installSignalHandlersFlag --launch_info is not specified
  */
  printf("yang test ............... main\r\n");
#ifdef __linux__
  int success = Subcommand::dispatch(
      "MESOS_CONTAINERIZER_",
      argc,
      argv,
      new MesosContainerizerLaunch(),
      new MesosContainerizerMount(),
      new NetworkCniIsolatorSetup());
#else
  int success = Subcommand::dispatch(
      "MESOS_CONTAINERIZER_",
      argc,
      argv,
      new MesosContainerizerLaunch(),
      new MesosContainerizerMount());
#endif

#ifdef __WINDOWS__
  if (!net::wsa_cleanup()) {
    EXIT(EXIT_FAILURE) << "Failed to cleanup the WSA socket stack";
  }
#endif // __WINDOWS__

  return success;
}
