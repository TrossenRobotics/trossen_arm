// Copyright 2025 Trossen Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef LIBTROSSEN_ARM__TROSSEN_ARM_UDP_CLIENT_HPP_
#define LIBTROSSEN_ARM__TROSSEN_ARM_UDP_CLIENT_HPP_

#include <netinet/in.h>
#include <sys/time.h>

#include <string>

#include "libtrossen_arm/trossen_arm_logging.hpp"

namespace trossen_arm
{

/// @brief UDP client class
class UDP_Client
{
private:
  // UDP packet size
  static constexpr uint16_t MAX_PACKET_SIZE {512};

  // Whether the UDP client is configured for the robot to be controlled
  // true if configured, false if not configured
  bool configured_ {false};

  // Socket file descriptor
  int sockfd_ {0};

  // Server address
  struct sockaddr_in servaddr_ {};

  // Server address length
  socklen_t servaddr_len_ { sizeof(sockaddr_in) };

  // Timeout
  timeval tv_ { .tv_sec = 0, .tv_usec = 0};

public:
  /**
   * @brief Destroy the udp client object
   */
  ~UDP_Client();

  /**
   * @brief Configure the UDP client
   *
   * @param serv_ip IP address of the server
   * @param port Port number of the server
   */
  void configure(const std::string serv_ip, uint16_t port);

  /**
   * @brief Cleanup the UDP client
   */
  void cleanup();

  /**
   * @brief Send data to the server
   *
   * @param size Size of the data
   */
  void send(size_t size);

  /**
   * @brief Receive data from the server
   *
   * @param timeout_us Timeout in microseconds for receiving UDP packets
   * @return ssize_t Size of the data received
   */
  ssize_t receive(uint32_t timeout_us);

  /**
   * @brief Send data to the server with guaranteed transmission
   *
   * @param size Size of the data
   * @param max_attempts Maximum number of retransmission attempts
   * @param timeout_us Timeout in microseconds for receiving UDP packets
   */
  void guaranteed_transmission(size_t size, uint8_t max_attempts, uint32_t timeout_us);

  /// @brief Send buffer
  uint8_t send_buffer[MAX_PACKET_SIZE];

  /// @brief Receive buffer
  uint8_t receive_buffer[MAX_PACKET_SIZE];
};

}  // namespace trossen_arm

#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_UDP_CLIENT_HPP_
