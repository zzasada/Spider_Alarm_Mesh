/*
 * Register Addresses for AK9756
 */
#ifndef _AK9756_REG_H
#define _AK9756_REG_H

#define AK9756_SLAVEADDR                                 0x64        // Currently: CAD0=0, CAD1=0
#define AK9756_REG_ADDR_WIA1                             0x00        // Manufacturer ID = 0x48
#define AK9756_REG_ADDR_WIA2                             0x01        // Product ID = 0x16
#define AK9756_REG_ADDR_INFO1                            0x02        // AKM Internal Info
#define AK9756_REG_ADDR_INFO2                            0x03        // AKM Internal Info
#define AK9756_REG_ADDR_ST1                              0x04        // Status1: D0=DRDY, D4=HBDR1
#define AK9756_REG_ADDR_IR1L                             0x05        // IR1 Data, Low Byte
#define AK9756_REG_ADDR_IR1H                             0x06        // IR1 Data, High Byte
#define AK9756_REG_ADDR_IR2L                             0x07        // IR2 Data, Low Byte
#define AK9756_REG_ADDR_IR2H                             0x08        // IR2 Data, High Byte
#define AK9756_REG_ADDR_TMPL                             0x09        // Temperature Data, Low Byte
#define AK9756_REG_ADDR_TMPH                             0x0A        // Temperature Data, High Byte
#define AK9756_REG_ADDR_ST2                              0x0B        // Status2: D0=DOR, D4=HBDR2,
#define AK9756_REG_ADDR_FIFOSTL                          0x0C        // FIFO Status, low byte
#define AK9756_REG_ADDR_FIFOSTH                          0x0D        // FIFO Status, high byte
#define AK9756_REG_ADDR_FIFODOR		                     0x0E        // FIFO Data Over Run (bit [0])
#define AK9756_REG_ADDR_FIFOD1L                          0x0F        // FIFO Data 1 Low Byte
#define AK9756_REG_ADDR_FIFOD1H                          0x10        // FIFO Data 1 High Byte
#define AK9756_REG_ADDR_FIFOD2L                          0x11        // FIFO Data 2 Low Byte
#define AK9756_REG_ADDR_FIFOD2H                          0x12        // FIFO Data 2 High Byte
#define AK9756_REG_ADDR_CNTL1                            0x13        // Control Register 1, D0=SRST, Soft Reset
#define AK9756_REG_ADDR_CNTL2                            0x14        // Control Register 2, D[6:5]=Continuous Mode, D[4:0]=IRGAIN
#define AK9756_REG_ADDR_CNTL3                            0x15        // Control Register 3, D[6:4]=ODR,D[3:2]= FCTMP, D[1:0]=FCIR
#define AK9756_REG_ADDR_CNTL4                            0x16        // Control Register 4, D[7:0]=HISTAT, H01IE, H10IE, HIE, WMIE, DIFFLHIE,DIFFHLIE, DRIE                                    
#define AK9756_REG_ADDR_CNTL5                            0x17        // Control Register 5, D[5]=FIFOLEN, D[4:3]=FIFODT[1:0], D[2:0]=FIFOM[2:0]
#define AK9756_REG_ADDR_CNTL6                            0x18        // Control Register 6, D[5:4]=FIFOSD[1:0], D[3:0]=WML[3:0]
#define AK9756_REG_ADDR_CNTL7                            0x19        // Control Register 7, D3=IRINV, D[2:0]=IDLET
#define AK9756_REG_ADDR_CNTL8                            0x1A        // Control Register 8, D[6:0]=H01THCNT[6:0] Detection Count Length
#define AK9756_REG_ADDR_CNTL9                            0x1B        // Control Register 9, D[5:0]=H10DIFTC[1:0], H10SUMTC[1:0], H10DIFE, H10SUME
#define AK9756_REG_ADDR_CNTL10                           0x1C        // Control Register 10, D[7:0]=H10THSEC[7:0]
// The following four 16-bit DIF and SUM upper and lower threshold values are UNSIGNED integers
#define AK9756_REG_ADDR_CNTL11                           0x1D        // Control Register 11, D[7:0]=DIFTHL[7:0]
#define AK9756_REG_ADDR_CNTL12                           0x1E        // Control Register 12, D[7:0]=DIFTHL[15:8]
#define AK9756_REG_ADDR_CNTL13                           0x1F        // Control Register 13, D[7:0]=DIFTHH[7:0]
#define AK9756_REG_ADDR_CNTL14                           0x20        // Control Register 14, D[7:0]=DIFTHH[15:8]
#define AK9756_REG_ADDR_CNTL15                           0x21        // Control Register 15, D[7:0]=SUMTHL[7:0]
#define AK9756_REG_ADDR_CNTL16                           0x22        // Control Register 16, D[7:0]=SUMTHL[15:8]
#define AK9756_REG_ADDR_CNTL17                           0x23        // Control Register 17, D[7:0]=SUMTHH[7:0]
#define AK9756_REG_ADDR_CNTL18                           0x24        // Control Register 18, D[7:0]=SUMTHH[15:8]

#define AK9756_REG_ADDR_CNTL19                           0x25		 // Control Register 19, D0=MODE
#define AK9756_REG_ADDR_CNTL20                           0x26		 // Control Register 20, Flex Mode Enable
#define AK9756_REG_ADDR_CNTL21                           0x27		 // Control Register 21, Flex Mode Duty

#define AK9756_REG_VALUE_WIA1                            0x48		 // Manufacturer Code (AKM)
#define AK9756_REG_VALUE_WIA2                            0x16		 // Device ID (AK9756)
 
#define AK9756_ST1_MASK_DIFTHHI                          0x10
#define AK9756_ST1_MASK_DIFTHLI                          0x08
#define AK9756_ST1_MASK_H10I                             0x04
#define AK9756_ST1_MASK_H01I                             0x02
#define AK9756_ST1_MASK_DRI                              0x01

#define AK9756_ST2_MASK_HRLST                            0x02        // Algorithm result. 0=Absent, 1=Present
#define AK9756_ST1_MASK_DOR                              0x01        // Data Overrun. 0=Normal, 1=Data Overrun

#define AK9756_REG_ADDR_INTCAUSE                         0x05        // Interrupt Cause: D[4:0]=DIFTHHI,DIFTHLI,H10I,H01I,DRI

/******************************************Control Registers***************************************/ 
//CNTL1 REG
#define CNTL1_SOFT_RESET                                  0xFF
#define CNTL1_RESET_OFF                                   0x00

//CNTL2 REG
#define CNTL2_CMODE1                                      0x80 // Continuous Mode 1 default
#define CNTL2_CMODE2                                      0xA0 // Continuous Mode 2
#define CNTL2_CMODE3                                      0xC0 // Continuous Mode 3
#define CNTL2_IRGAIN_DEF                                  0x9A //default
#define CNTL2_IRGAIN_MIN                                  0x90
#define CNTL2_IRGAIN_MAX                                  0x8F
#define CNTL2_IRGAIN_135                                  0x01 // 135%
#define CNTL2_IRGAIN_130                                  0x00 // 130%
#define CNTL2_IRGAIN_125                                  0x1F // 125%
#define CNTL2_IRGAIN_120                                  0x1E // 120%
#define CNTL2_IRGAIN_115                                  0x1D // 115%
#define CNTL2_IRGAIN_110                                  0x1C // 110%
#define CNTL2_IRGAIN_CUST                                 0x88 // Use 170% IR gain to compensate for lens IR blockage

#define CNTL2_DEFAULT                                     (CNTL2_CMODE1 | CNTL2_IRGAIN_DEF) // 0x9A
#define CNTL2_DEMO1                                       (CNTL2_CMODE1 | CNTL2_IRGAIN_DEF) // 0x9A
#define CNTL2_DEMOLENS                                    (CNTL2_CMODE1 | CNTL2_IRGAIN_CUST) // 0x9A (assuming default gain))
#define CNTL2_DEMO2                                       (CNTL2_CMODE2 | CNTL2_IRGAIN_DEF) // 0xBA
#define CNTL2_DEMO3                                       (CNTL2_CMODE3 | CNTL2_IRGAIN_DEF) //

//CNTL3 REG
#define CNTL3_ODR_1HZ_MODE                                0x80
#define CNTL3_ODR_5HZ_MODE                                0x90
#define CNTL3_ODR_10HZ_MODE                               0xA0 //default
#define CNTL3_ODR_25HZ_MODE                               0xB0
#define CNTL3_ODR_50HZ_MODE                               0xC0
#define CNTL3_ODR_100HZ_MODE                              0xD0
#define CNTL3_ODR_400HZ_MODE                              0xE0

#define CNTL3_FCTMP_NO_FC                                 0x00 //default
#define CNTL3_FCTMP_ODRD3_FC                              0x04
#define CNTL3_FCTMP_ODRD6_FC                              0x08

#define CNTL3_FCIR_NO_FC                                  0x00 //default
#define CNTL3_FCIR_ODRD3_FC                               0x01
#define CNTL3_FCIR_ODRD6_FC                               0x02
#define CNTL3_DEFAULT       (CNTL3_ODR_10HZ_MODE | CNTL3_FCTMP_NO_FC | CNTL3_FCIR_NO_FC)
#define CNTL3_LOG_25HZ       (CNTL3_ODR_25HZ_MODE | CNTL3_FCTMP_NO_FC | CNTL3_FCIR_NO_FC)
#define CNTL3_DEMO_10HZ       (CNTL3_ODR_10HZ_MODE | CNTL3_FCTMP_ODRD6_FC | CNTL3_FCIR_ODRD6_FC)

//CNTL4 REG
#define CNTL4_HISTAT_AIS_PRESENT                          0x80 //default
#define CNTL4_HISTAT_AIS_ABSENT                           0x00

#define CNTL4_HD01E_DISABLE                               0x00 //default
#define CNTL4_HD01E_ENABLE                                0x40

#define CNTL4_HD10E_DISABLE                               0x00 //default
#define CNTL4_HD10E_ENABLE                                0x20

#define CNTL4_HDIE_DISABLE                                0x00 //default
#define CNTL4_HDIE_ENABLE                                 0x10

#define CNTL4_WMIE_DISABLE                                0x00 //default
#define CNTL4_WMIE_ENABLE                                 0x08

#define CNTL4_DIFFUTIE_DISABLE                            0x00 //default
#define CNTL4_DIFFUTIE_ENABLE                             0x04

#define CNTL4_DIFFLTIE_DISABLE                            0x00 //default
#define CNTL4_DIFFLTIE_ENABLE                             0x02

#define CNTL4_DRIE_DISABLE                                0x00 //default
#define CNTL4_DRIE_ENABLE                                 0x01

#define CNTL4_DEFAULT (CNTL4_HISTAT_AIS_PRESENT | CNTL4_HD01E_DISABLE | CNTL4_HD10E_DISABLE | CNTL4_HDIE_DISABLE | CNTL4_WMIE_DISABLE | CNTL4_DIFFUTIE_DISABLE | CNTL4_DIFFLTIE_DISABLE | CNTL4_DRIE_DISABLE)
#define CNTL4_LOG_DATA (CNTL4_HISTAT_AIS_PRESENT | CNTL4_HD01E_DISABLE | CNTL4_HD10E_DISABLE | CNTL4_HDIE_DISABLE | CNTL4_WMIE_DISABLE | CNTL4_DIFFUTIE_DISABLE | CNTL4_DIFFLTIE_DISABLE | CNTL4_DRIE_ENABLE)
#define CNTL4_HPD_DEMO (CNTL4_HISTAT_AIS_PRESENT | CNTL4_HD01E_ENABLE | CNTL4_HD10E_ENABLE | CNTL4_HDIE_ENABLE | CNTL4_WMIE_DISABLE | CNTL4_DIFFUTIE_DISABLE | CNTL4_DIFFLTIE_DISABLE | CNTL4_DRIE_DISABLE)

//CNTL5 REG
#define CNTL5_FIFOLEN_LONG                                 0xC0 //default
#define CNTL5_FIFOLEN_SHORT                                0xE0

#define CNTL5_FIFODT_RAW                                   0xC0 //default
#define CNTL5_FIFODT_DIFF                                  0xC8
#define CNTL5_FIFODT_AVG                                   0xD0

#define CNTL5_FIFOM_BYPASS                                 0xC0 //default
#define CNTL5_FIFOM_STOP                                   0xC4
#define CNTL5_FIFOM_STREAM                                 0xC5
#define CNTL5_FIFOM_STREAMSTOP                             0xC6
#define CNTL5_DEFAULT (CNTL5_FIFOLEN_LONG | CNTL5_FIFODT_RAW | CNTL5_FIFOM_BYPASS)
#define CNTL5_HPD_DEMO (CNTL5_FIFOLEN_SHORT | CNTL5_FIFODT_AVG | CNTL5_FIFOM_STOP)

//CNTL6 REG
#define CNTL6_FIFOSD_INSTANT                               0xC0 //default
#define CNTL6_FIFOSD_14                                    0xD0
#define CNTL6_FIFOSD_24                                    0xE0
#define CNTL6_FIFOSD_34                                    0xF0

#define CNTL6_WML_NONE                                     0x00 //default
#define CNTL6_WML_4                                        0x01
#define CNTL6_WML_8                                        0x02
#define CNTL6_WML_16                                       0x03
#define CNTL6_WML_32                                       0x04
#define CNTL6_WML_64                                       0x05
#define CNTL6_WML_128                                      0x06
#define CNTL6_WML_200                                      0x07
#define CNTL6_WML_300                                      0x08
#define CNTL6_WML_400                                      0x09
#define CNTL6_WML_500                                      0x0A
#define CNTL6_DEFAULT                                      (CNTL6_FIFOSD_INSTANT | CNTL6_WML_NONE) // 0xC0
#define CNTL6_HPD_DEMO                                     (CNTL6_FIFOSD_INSTANT | CNTL6_WML_32)

//CNTL7 REG
#define CNTL7_H01DIFTC_15S                                 0xC0
#define CNTL7_H01DIFTC_30S                                 0xD0 //default 
#define CNTL7_H01DIFTC_60S                                 0xE0
#define CNTL7_H01DIFTC_120S                                0xF0

#define CNTL7_H01SUMTC_5S                                  0x00
#define CNTL7_H01SUMTC_10S                                 0x04 //default
#define CNTL7_H01SUMTC_20S                                 0x08
#define CNTL7_H01SUMTC_40S                                 0x0C

#define CNTL7_H01DIFE_ENABLE                               0x02 //default
#define CNTL7_H01DIFE_DISABLE                              0x00

#define CNTL7_H01SUME_ENABLE                               0x01 //default
#define CNTL7_H01SUME_DISABLE                              0x00 

#define CNTL7_DEFAULT   (CNTL7_H01DIFTC_30S | CNTL7_H01SUMTC_10S | CNTL7_H01DIFE_ENABLE | CNTL7_H01SUME_ENABLE) // 0xD7
#define CNTL7_HPD_DEMO  (CNTL7_H01DIFTC_30S | CNTL7_H01SUMTC_10S | CNTL7_H01DIFE_ENABLE | CNTL7_H01SUME_ENABLE)

//CNTL8 REG
#define CNTL8_H01THCNT_DEF                                 0x80 //default

#define CNTL8_DEFAULT                                      (CNTL8_H01THCNT_DEF)
#define CNTL8_HPD_DEMO                                     0x85 //detection count of 6

//CNTL9 REG
#define CNTL9_H10DIFTC_15S                                 0x00
#define CNTL9_H10DIFTC_30S                                 0x10 //default
#define CNTL9_H10DIFTC_60S                                 0x20
#define CNTL9_H10DIFTC_120S                                0x30

#define CNTL9_H10SUMTC_5S                                  0x00
#define CNTL9_H10SUMTC_10S                                 0x40 //default
#define CNTL9_H10SUMTC_20S                                 0x80
#define CNTL9_H10SUMTC_40S                                 0xC0

#define CNTL9_H10DIFE_ENABLE                               0x02 //default
#define CNTL9_H10DIFE_DISABLE                              0x00

#define CNTL9_H10SUME_ENABLE                               0x01 //default
#define CNTL9_H10SUME_DISABLE                              0x00

#define CNTL9_DEFAULT                                      (CNTL9_H10DIFTC_30S | CNTL9_H10SUMTC_10S | CNTL9_H10DIFE_ENABLE | CNTL9_H10SUME_ENABLE)
#define CNTL9_HPD_DEMO                                     (CNTL9_H10DIFTC_30S | CNTL9_H10SUMTC_10S | CNTL9_H10DIFE_ENABLE | CNTL9_H10SUME_ENABLE)

//CNTL10 REG
#define CNTL10_H10THSEC_DEF                                0x90 //default = 256 seconds
#define CNTL10_H10THSEC_4S                                 0x41 //4 seconds, (1 x 2 squared seconds)
#define CNTL10_DEFAULT                                     (CNTL10_H10THSEC_DEF)
#define CNTL10_HPD_DEMO                                    (CNTL10_H10THSEC_4S)

//CNTL11 REG
#define CNTL11_DIFTHLL_170                                 0xAA // 170
#define CNTL11_DIFTHLL_DEFAULT                             0x96 //default 150
#define CNTL11_DIFTHLL_140                                 0x8C // 140
#define CNTL11_DIFTHLL_130                                 0x82 // 130
#define CNTL11_DIFTHLL_120                                 0x78 // 120
#define CNTL11_DIFTHLL_100                                 0x64 // 100
#define CNTL11_DEFAULT                                     (CNTL11_DIFTHLL_DEFAULT)
// CNTL12 REG
#define CNTL12_DIFTHLU_DEFAULT                             0x00 //default
#define CNTL12_DEFAULT                                     (CNTL12_DIFTHLU_DEFAULT)

//CNTL13 REG
#define CNTL13_DIFTHHL_170                                 0xAA // 170
#define CNTL13_DIFTHHL_DEFAULT                             0x96 //default
#define CNTL13_DIFTHHL_140                                 0x8C // 140
#define CNTL13_DIFTHHL_130                                 0x82 // 130
#define CNTL13_DIFTHHL_120                                 0x78 // 120
#define CNTL13_DIFTHHL_100                                 0x64 // 100
#define CNTL13_DEFAULT                                     (CNTL13_DIFTHHL_DEFAULT)
// CNTL14 REG
#define CNTL14_DIFTHHU_DEFAULT                             0x00 //default
#define CNTL14_DEFAULT                                     (CNTL14_DIFTHHU_DEFAULT)

//CNTL15 REG
#define CNTL15_SUMTHLL_170                                 0xAA // 170
#define CNTL15_SUMTHLL_DEFAULT                             0x96 //default 150
#define CNTL15_SUMTHLL_140                                 0x8C // 140
#define CNTL15_SUMTHLL_130                                 0x82 // 130
#define CNTL15_SUMTHLL_120                                 0x78 // 120
#define CNTL15_SUMTHLL_100                                 0x64 // 100
#define CNTL15_DEFAULT                                     (CNTL15_SUMTHLL_DEFAULT)
// CNTL16 REG
#define CNTL16_SUMTHLU_DEFAULT                             0x00 //default
#define CNTL16_DEFAULT                                     (CNTL16_SUMTHLU_DEFAULT)

//CNTL17 REG
#define CNTL17_SUMTHHL_170                                 0xAA // 170
#define CNTL17_SUMTHHL_DEFAULT                             0x96 //default
#define CNTL17_SUMTHHL_140                                 0x8C // 140
#define CNTL17_SUMTHHL_130                                 0x82 // 130
#define CNTL17_SUMTHHL_120                                 0x78 // 120
#define CNTL17_SUMTHHL_100                                 0x64 // 100
#define CNTL17_DEFAULT                                     (CNTL17_SUMTHHL_DEFAULT)
// CNTL18 REG
#define CNTL18_SUMTHHU_DEFAULT                             0x00 //default
#define CNTL18_DEFAULT                                     (CNTL18_SUMTHHU_DEFAULT)

//CNTL19 REG
#define CNTL19_MODE_STANDBY                                 0xFE
#define CNTL19_MODE_CONTINUOUS                              0xFF

#endif