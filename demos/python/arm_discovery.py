# Copyright 2026 Trossen Robotics
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#    * Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Purpose:
# This script discovers all arm controllers on a given subnet.

# Hardware setup:
# 1. Arms on the 192.168.1.x subnet

# The script does the following:
# 1. Scans the given subnet range for arm controllers
# 2. Prints the IP, model, firmware version, and error state of each found arm

import trossen_arm

if __name__=='__main__':

    subnet = "192.168.1"
    ip_start = 1
    ip_end = 254
    timeout = 0.01

    print(
        f"Scanning {subnet}.{ip_start}"
        f" - {subnet}.{ip_end}"
        f" (timeout={timeout}s)..."
    )

    discover_results = trossen_arm.TrossenArmDriver.discover(
        subnet=subnet,
        ip_start=ip_start,
        ip_end=ip_end,
        timeout=timeout,
    )

    print(f"\nFound {len(discover_results)} arm(s):")
    for discover_result in discover_results:
        print(
            f"  ip={discover_result.ip}"
            f"  model="
            f"{trossen_arm.MODEL_NAME[discover_result.model]}"
            f"  firmware={discover_result.firmware_version}"
            f"  error_state="
            f"{trossen_arm.ERROR_INFORMATION[discover_result.error_state]}"
        )
