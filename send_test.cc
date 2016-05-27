// Copyright 2016 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "send.h"
#include "flow.h"
#include "util.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

namespace clerk {
namespace ipfix {

class SendTest : public ::testing::Test {};

void PrintPacket(StringPiece p) {
  if (VLOG_IS_ON(1)) {
    printf("const char packet[] = {");
    for (size_t i = 0; i < p.size(); i++) {
      if (i % 8 == 0) {
        printf("\n\t");
      }
      printf("0x%02X, ", uint8_t(p[i]));
    }
    printf("\n};\n");
  }
}

TEST_F(SendTest, TemplateV4Packet) {
  const char want[] = {
      0x00, 0x0A, 0x00, 0x58, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00,
      0x03, 0x00, 0x00, 0x30, 0x39, 0x00, 0x02, 0x00, 0x48, 0x01, 0x00,
      0x00, 0x10, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x04, 0x00,
      0x07, 0x00, 0x02, 0x00, 0x0B, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01,
      0x00, 0x06, 0x00, 0x01, 0x00, 0x20, 0x00, 0x02, 0x00, 0x10, 0x00,
      0x04, 0x00, 0x11, 0x00, 0x04, 0x00, 0x01, 0x00, 0x08, 0x00, 0x02,
      0x00, 0x08, 0x00, 0x9C, 0x00, 0x08, 0x00, 0x9D, 0x00, 0x08, 0x00,
      0x05, 0x00, 0x01, 0x00, 0x88, 0x00, 0x01, 0x00, 0x3A, 0x00, 0x02,
  };
  IPFIXPacket p(222);
  p.Reset(PT_TEMPLATE, 3);
  p.WriteFlowSet(true);
  auto data = p.PacketData();
  PrintPacket(data);
  ASSERT_EQ(data, StringPiece(want, sizeof(want)));
}

TEST_F(SendTest, TemplateV6Packet) {
  const char want[] = {
      0x00, 0x0A, 0x00, 0x58, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00,
      0x03, 0x00, 0x00, 0x30, 0x39, 0x00, 0x02, 0x00, 0x48, 0x01, 0x01,
      0x00, 0x10, 0x00, 0x1B, 0x00, 0x10, 0x00, 0x1C, 0x00, 0x10, 0x00,
      0x07, 0x00, 0x02, 0x00, 0x0B, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01,
      0x00, 0x06, 0x00, 0x01, 0x00, 0x20, 0x00, 0x02, 0x00, 0x10, 0x00,
      0x04, 0x00, 0x11, 0x00, 0x04, 0x00, 0x01, 0x00, 0x08, 0x00, 0x02,
      0x00, 0x08, 0x00, 0x9C, 0x00, 0x08, 0x00, 0x9D, 0x00, 0x08, 0x00,
      0x05, 0x00, 0x01, 0x00, 0x88, 0x00, 0x01, 0x00, 0x3A, 0x00, 0x02,
  };
  IPFIXPacket p(222);
  p.Reset(PT_TEMPLATE, 3);
  p.WriteFlowSet(false);
  auto data = p.PacketData();
  PrintPacket(data);
  ASSERT_EQ(data, StringPiece(want, sizeof(want)));
}

TEST_F(SendTest, DataV4Packet) {
  const char want[] = {
      // header
      0x00, 0x0A, 0x00, 0x8C, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00, 0x03,
      0x00, 0x00, 0x30, 0x39,
      // record set
      0x01, 0x00, 0x00, 0x7C,
      // record 1
      0x11, 0x22, 0x33, 0x44, 0xAA, 0xBB, 0xCC, 0xDD, 0x99, 0x99, 0xAA, 0xAA,
      0x00, 0x00, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x88,
      0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0xAB, 0xFF, 0xFF,
      // record 2
      0x11, 0x22, 0x33, 0x44, 0xAA, 0xBB, 0xCC, 0xDD, 0x99, 0x99, 0xAA, 0xAA,
      0x00, 0x00, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x88,
      0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE, 0xCD, 0xFF, 0xFF,
  };
  flow::Key k;
  flow::Stats s;
  k.set_src_ip4(0x11223344);
  k.set_dst_ip4(0xAABBCCDD);
  k.src_port = 0x9999;
  k.dst_port = 0xAAAA;
  k.tos = 0xEE;
  k.vlan = 0xFFFF;
  k.icmp_type = 0x55;
  k.icmp_code = 0x66;
  s.bytes = 0x7777777777LL;
  s.packets = 0x8888888888LL;
  IPFIXPacket p(222);
  p.Reset(PT_V4, 3);
  p.AddToBuffer(k, s, 0xAB);
  p.AddToBuffer(k, s, 0xCD);
  auto data = p.PacketData();
  PrintPacket(data);
  ASSERT_EQ(data, StringPiece(want, sizeof(want)));
}

}  // namespace ipfix
}  // namespace clerk
