#include <stdio.h>

#include "svdpi.h"
#include "../dpiheader.h"

// define RTC address values
#define RTC_CTRL            0x00000000
#define RTC_NULL_0x4        0x00000004
#define RTC_NULL_0x8        0x00000008
#define RTC_NULL_0xC        0x0000000C
#define RTC_TIME_SEC_H_LOAD 0x00000010
#define RTC_TIME_SEC_L_LOAD 0x00000014
#define RTC_TIME_NSC_H_LOAD 0x00000018
#define RTC_TIME_NSC_L_LOAD 0x0000001C
#define RTC_PERIOD_H_LOAD   0x00000020
#define RTC_PERIOD_L_LOAD   0x00000024
#define RTC_ACCMOD_H_LOAD   0x00000028
#define RTC_ACCMOD_L_LOAD   0x0000002C
#define RTC_ADJNUM_LOAD     0x00000030
#define RTC_NULL_0x34       0x00000034
#define RTC_ADJPER_H_LOAD   0x00000038
#define RTC_ADJPER_L_LOAD   0x0000003C
#define RTC_TIME_SEC_H_READ 0x00000040
#define RTC_TIME_SEC_L_READ 0x00000044
#define RTC_TIME_NSC_H_READ 0x00000048
#define RTC_TIME_NSC_L_READ 0x0000004C
// define RTC data values
#define RTC_SET_CTRL_0 0x0
#define RTC_GET_TIME   0x1
#define RTC_SET_ADJ    0x2
#define RTC_SET_PERIOD 0x4
#define RTC_SET_TIME   0x8
#define RTC_SET_RESET  0x10
#define RTC_ACCMOD_H   0x3B9ACA00  // 1,000,000,000 for 30bit
#define RTC_ACCMOD_L   0x0         // 256 for 8bit
#define RTC_PERIOD_H   0x8  // 8ns for 125MHz rtc_clk
#define RTC_PERIOD_L   0x0

// define TSU address values
#define TSU_CTRL         0x00000050
#define TSU_RXQUE_STATUS 0x00000054
#define TSU_TXQUE_STATUS 0x00000058
#define TSU_NULL_0x5C    0x0000005C
#define TSU_RXQUE_DATA_H 0x00000060
#define TSU_RXQUE_DATA_L 0x00000064
#define TSU_TXQUE_DATA_H 0x00000068
#define TSU_TXQUE_DATA_L 0x0000006C
// define TSU data values
#define TSU_SET_CTRL_0 0x0
#define TSU_GET_TXQUE  0x1
#define TSU_GET_RXQUE  0x4
#define TSU_SET_RESET  0xA

int ptp_drv_bfm_c(double fw_delay)
{
  unsigned int cpu_addr_i;
  unsigned int cpu_data_i;
  unsigned int cpu_data_o;

  // LOAD RTC PERIOD AND ACC_MODULO
  cpu_addr_i = RTC_PERIOD_H_LOAD;
  cpu_data_i = RTC_PERIOD_H;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_PERIOD_L_LOAD;
  cpu_data_i = RTC_PERIOD_L;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_ACCMOD_H_LOAD;
  cpu_data_i = RTC_ACCMOD_H;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_ACCMOD_L_LOAD;
  cpu_data_i = RTC_ACCMOD_L;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_PERIOD;
  cpu_wr(cpu_addr_i, cpu_data_i);
  // RESET RTC AND TSU
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_RESET;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = TSU_CTRL;
  cpu_data_i = TSU_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = TSU_CTRL;
  cpu_data_i = TSU_SET_RESET;
  cpu_wr(cpu_addr_i, cpu_data_i);
  // READ RTC SEC AND NS
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_GET_TIME;
  cpu_wr(cpu_addr_i, cpu_data_i);
  do {
    cpu_addr_i = RTC_CTRL;
    cpu_rd(cpu_addr_i, &cpu_data_o);
    //printf("%08x\n", (cpu_data_o & 0x1));
  } while ((cpu_data_o & RTC_GET_TIME) == 0x0);
  cpu_addr_i = RTC_TIME_SEC_H_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("\ntime: \n%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_SEC_L_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_NSC_H_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_NSC_L_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);
  // LOAD RTC SEC AND NS
  cpu_addr_i = RTC_TIME_SEC_H_LOAD;
  cpu_data_i = 0x0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_TIME_SEC_L_LOAD;
  cpu_data_i = 0x1;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_TIME_NSC_H_LOAD;
  cpu_data_i = RTC_ACCMOD_H - 0xA;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_TIME_NSC_L_LOAD;
  cpu_data_i = 0x0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_TIME;
  cpu_wr(cpu_addr_i, cpu_data_i);
  // LOAD RTC ADJ
  cpu_addr_i = RTC_ADJNUM_LOAD;
  cpu_data_i = 0x100;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_ADJPER_H_LOAD;
  cpu_data_i = 0x1;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_ADJPER_L_LOAD;
  cpu_data_i = 0x20;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_ADJ;
  cpu_wr(cpu_addr_i, cpu_data_i);
  // READ RTC SEC AND NS
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_SET_CTRL_0;
  cpu_wr(cpu_addr_i, cpu_data_i);
  cpu_addr_i = RTC_CTRL;
  cpu_data_i = RTC_GET_TIME;
  cpu_wr(cpu_addr_i, cpu_data_i);
  do {
    cpu_addr_i = RTC_CTRL;
    cpu_rd(cpu_addr_i, &cpu_data_o);
    //printf("%08x\n", (cpu_data_o & 0x1));
  } while ((cpu_data_o & RTC_GET_TIME) == 0x0);
  cpu_addr_i = RTC_TIME_SEC_H_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("\ntime: \n%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_SEC_L_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_NSC_H_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);
  cpu_addr_i = RTC_TIME_NSC_L_READ;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  printf("%08x\n", cpu_data_o);

  int i;
  int rx_queue_num;
  int tx_queue_num;
  while (1) {
  // POLL TSU RX STATUS
  cpu_addr_i = TSU_RXQUE_STATUS;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  rx_queue_num = cpu_data_o;
  //printf("%08x\n", rx_queue_num);
  if (rx_queue_num > 0x0) {
    // READ TSU RX FIFO
    for (i=rx_queue_num; i>0; i--) {
      cpu_addr_i = TSU_CTRL;
      cpu_data_i = TSU_SET_CTRL_0;
      cpu_wr(cpu_addr_i, cpu_data_i);
      cpu_addr_i = TSU_CTRL;
      cpu_data_i = TSU_GET_RXQUE;
      cpu_wr(cpu_addr_i, cpu_data_i);
      do {
        cpu_addr_i = TSU_CTRL;
        cpu_rd(cpu_addr_i, &cpu_data_o);
        //printf("%08x\n", (cpu_data_o & 0x1));
      } while ((cpu_data_o & TSU_GET_RXQUE) == 0x0);
      cpu_addr_i = TSU_RXQUE_DATA_H;
      cpu_rd(cpu_addr_i, &cpu_data_o);
      printf("\nRx stamp: \n%08x\n", cpu_data_o);
      cpu_addr_i = TSU_RXQUE_DATA_L;
      cpu_rd(cpu_addr_i, &cpu_data_o);
      printf("%08x\n", cpu_data_o);
    }
  }
  // POLL TSU TX STATUS
  cpu_addr_i = TSU_TXQUE_STATUS;
  cpu_rd(cpu_addr_i, &cpu_data_o);
  tx_queue_num = cpu_data_o;
  //printf("%08x\n", tx_queue_num);
  if (tx_queue_num > 0x0) {
    // READ TSU TX FIFO
    for (i=tx_queue_num; i>0; i--) {
      cpu_addr_i = TSU_CTRL;
      cpu_data_i = TSU_SET_CTRL_0;
      cpu_wr(cpu_addr_i, cpu_data_i);
      cpu_addr_i = TSU_CTRL;
      cpu_data_i = TSU_GET_TXQUE;
      cpu_wr(cpu_addr_i, cpu_data_i);
      do {
        cpu_addr_i = TSU_CTRL;
        cpu_rd(cpu_addr_i, &cpu_data_o);
        //printf("%08x\n", (cpu_data_o & 0x1));
      } while ((cpu_data_o & TSU_GET_TXQUE) == 0x0);
      cpu_addr_i = TSU_TXQUE_DATA_H;
      cpu_rd(cpu_addr_i, &cpu_data_o);
      printf("\nTx stamp: \n%08x\n", cpu_data_o);
      cpu_addr_i = TSU_TXQUE_DATA_L;
      cpu_rd(cpu_addr_i, &cpu_data_o);
      printf("%08x\n", cpu_data_o);
    }
  }
  }

  // READ BACK ALL REGISTERS
  for (;;)
  {
    int t;
    for (t=0; t<=0x5c; t=t+4) 
    {
      cpu_hd(10);

      cpu_addr_i = t;
      cpu_rd(cpu_addr_i, &cpu_data_o);
    }
  }

  return(0); /* Return success (required by tasks) */
}
