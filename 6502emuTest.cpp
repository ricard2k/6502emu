#include "gtest/gtest.h"

#include "6502emu.h"


class EmuTest : public ::testing::Test {
  protected:
    virtual void SetUp() { cpu.reset(); }
    CPU cpu;
    Memory memory;
};


TEST_F(EmuTest, LdaInmediateTest){

  //Given
  CPU cpuCopy = cpu;
  memory.write8(0xFFFC, 0xA9);
  memory.write8(0xFFFD, 0x05);

  //When
  cpuCopy.execute(memory);

  //Then
  EXPECT_EQ(0x05, cpuCopy.A);
  EXPECT_EQ(cpu.X, cpuCopy.X);
  EXPECT_EQ(cpu.Y, cpuCopy.Y);

  EXPECT_EQ(cpu.C, cpuCopy.C);
  EXPECT_EQ(cpu.Z, cpuCopy.Z);
  EXPECT_EQ(cpu.I, cpuCopy.I);
  EXPECT_EQ(cpu.D, cpuCopy.D);
  EXPECT_EQ(cpu.B, cpuCopy.B);
  EXPECT_EQ(cpu.V, cpuCopy.V);
  EXPECT_EQ(cpu.N, cpuCopy.N);

  EXPECT_EQ(cpu.SP, cpuCopy.SP);
  EXPECT_EQ(cpu.PC + 2, cpuCopy.PC);
}

TEST_F(EmuTest, LdaInmediateTestWithZero){

  //Given
  CPU cpuCopy = cpu;
  memory.write8(0xFFFC, 0xA9);
  memory.write8(0xFFFD, 0x00);

  //When
  cpuCopy.execute(memory);

  //Then
  EXPECT_EQ(0x00, cpuCopy.A);
  EXPECT_EQ(cpu.X, cpuCopy.X);
  EXPECT_EQ(cpu.Y, cpuCopy.Y);

  EXPECT_EQ(cpu.C, cpuCopy.C);
  EXPECT_EQ(true, cpuCopy.Z);
  EXPECT_NE(cpu.Z, cpuCopy.Z);
  
  EXPECT_EQ(cpu.I, cpuCopy.I);
  EXPECT_EQ(cpu.D, cpuCopy.D);
  EXPECT_EQ(cpu.B, cpuCopy.B);
  EXPECT_EQ(cpu.V, cpuCopy.V);
  EXPECT_EQ(cpu.N, cpuCopy.N);

  EXPECT_EQ(cpu.SP, cpuCopy.SP);
  EXPECT_EQ(cpu.PC + 2, cpuCopy.PC);
}

TEST_F(EmuTest, LdaInmediateTestWithNegative){

  //Given
  CPU cpuCopy = cpu;
  memory.write8(0xFFFC, 0xA9);
  memory.write8(0xFFFD, 0xFF);

  //When
  cpuCopy.execute(memory);

  //Then
  EXPECT_EQ(0xFF, cpuCopy.A);
  EXPECT_EQ(cpu.X, cpuCopy.X);
  EXPECT_EQ(cpu.Y, cpuCopy.Y);

  EXPECT_EQ(cpu.C, cpuCopy.C);
  EXPECT_EQ(cpu.Z, cpuCopy.Z);
  EXPECT_EQ(cpu.I, cpuCopy.I);
  EXPECT_EQ(cpu.D, cpuCopy.D);
  EXPECT_EQ(cpu.B, cpuCopy.B);
  EXPECT_EQ(cpu.V, cpuCopy.V);
  EXPECT_NE(cpu.N, cpuCopy.N);
  EXPECT_EQ(true, cpuCopy.N);

  EXPECT_EQ(cpu.SP, cpuCopy.SP);
  EXPECT_EQ(cpu.PC + 2, cpuCopy.PC);
}