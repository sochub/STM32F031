/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *文件名:  lib_rtc.c
 *作  者:  ESM Application Team
 *版  本:  V1.01
 *日  期:  2016/11/01
 *描  述:  RTC模块库函数
 *备  注:
 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
 **********************************************************/
#include "lib_rtc.h"

/***************************************************************
  函数名：RTC_Init
  描  述：实时时钟初始化
  输入值：CLKx:RTC时钟源选择；HOURx：12小时/24小时制选择
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_Init(RTC_TYPE_CLKS CLKx, RTC_TYPE_TIME HOURx)
{
  uint32_t flag;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  RTC->CON.PON = 0;       // 清零，使能RTC
  RTC->CON.HSWI = (unsigned int)HOURx;    // 模式选择
  RTC->CON.CLKS = (unsigned int)CLKx; // 时钟源选择

  RTC->CAL.CALF = 0;  // 调校值为0
  RTC->CAL.DEV = 1;       // 每60秒调校一次
  RTC->CAL.COCR = 3;  // 输出脉冲为1Hz
  RTC->CAL.CLKC = 0;  // 输出脉冲端口禁止

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;
}

/***************************************************************
  函数名：RTC_StartRead
  描  述：启动实时时钟读流程
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_StartRead(void)
{
  RTC->CON.TMWR = 0;  // 操作类型是读
  RTC->CON.TMUP = 1;  // 触发读操作

  while (RTC->CON.TMUP == 1);
}

/***************************************************************
  函数名：RTC_ReadHourmode
  描  述：读取小时模式
  输入值：无
  输出值：无
  返回值：当前的小时模式
 ***************************************************************/
uint32_t RTC_ReadHourmode(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  result = RTC->CON.HSWI;

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}
/***************************************************************
  函数名：RTC_ReadSecond
  描  述：读取秒
  输入值：无
  输出值：无
  返回值：当前的秒
 ***************************************************************/
uint32_t RTC_ReadSecond(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->HMS.SEC & (uint32_t)0x01) * 1);
  result += (((RTC->HMS.SEC >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->HMS.SEC >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->HMS.SEC >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->HMS.SEC >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->HMS.SEC >> 5) & (uint32_t)0x01) * 20);
  result += (((RTC->HMS.SEC >> 6) & (uint32_t)0x01) * 40);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadMinute
  描  述：读取分钟
  输入值：无
  输出值：无
  返回值：当前的分钟
 ***************************************************************/
uint32_t RTC_ReadMinute(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->HMS.MIN & (uint32_t)0x01) * 1);
  result += (((RTC->HMS.MIN >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->HMS.MIN >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->HMS.MIN >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->HMS.MIN >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->HMS.MIN >> 5) & (uint32_t)0x01) * 20);
  result += (((RTC->HMS.MIN >> 6) & (uint32_t)0x01) * 40);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadHour
  描  述：读取小时
  输入值：无
  输出值：无
  返回值：当前的小时
 ***************************************************************/
uint32_t RTC_ReadHour(uint32_t *meridiem)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->HMS.HOUR & (uint32_t)0x01) * 1);
  result += (((RTC->HMS.HOUR >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->HMS.HOUR >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->HMS.HOUR >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->HMS.HOUR >> 4) & (uint32_t)0x01) * 10);

  if (RTC_ReadHourmode() == RTC_HOUR24)                       //24小时模式：20小时
  {
    result += (((RTC->HMS.HOUR >> 5) & (uint32_t)0x01) * 20);
  }
  else                                                        //12小时模式：1 PM,0 AM
  {
    *meridiem = (((RTC->HMS.HOUR >> 5) & (uint32_t)0x01));
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadDay
  描  述：读取日
  输入值：无
  输出值：无
  返回值：当前的日
 ***************************************************************/
uint32_t RTC_ReadDay(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->YMDW.DAY & (uint32_t)0x01) * 1);
  result += (((RTC->YMDW.DAY >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->YMDW.DAY >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->YMDW.DAY >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->YMDW.DAY >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->YMDW.DAY >> 5) & (uint32_t)0x01) * 20);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadMonth
  描  述：读取月
  输入值：无
  输出值：无
  返回值：当前的月
 ***************************************************************/
uint32_t RTC_ReadMonth(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->YMDW.MON & (uint32_t)0x01) * 1);
  result += (((RTC->YMDW.MON >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->YMDW.MON >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->YMDW.MON >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->YMDW.MON >> 4) & (uint32_t)0x01) * 10);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadYear
  描  述：读取年
  输入值：无
  输出值：无
  返回值：当前的年
 ***************************************************************/
uint32_t RTC_ReadYear(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->YMDW.YEAR & (uint32_t)0x01) * 1);
  result += (((RTC->YMDW.YEAR >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->YMDW.YEAR >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->YMDW.YEAR >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->YMDW.YEAR >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->YMDW.YEAR >> 5) & (uint32_t)0x01) * 20);
  result += (((RTC->YMDW.YEAR >> 6) & (uint32_t)0x01) * 40);
  result += (((RTC->YMDW.YEAR >> 7) & (uint32_t)0x01) * 80);
  result += 2000;

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadWeek
  描  述：读取星期
  输入值：无
  输出值：无
  返回值：当前的星期
 ***************************************************************/
uint32_t RTC_ReadWeek(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  switch (RTC->YMDW.WEAK)
  {
    case 0x01:
      result = 0;
      break;

    case 0x02:
      result = 1;
      break;

    case 0x04:
      result = 2;
      break;

    case 0x08:
      result = 3;
      break;

    case 0x10:
      result = 4;
      break;

    case 0x20:
      result = 5;
      break;

    case 0x40:
      result = 6;
      break;

    default:
      result = 0;
      break;
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_WriteSecond
  描  述：修改秒
  输入值：second: 秒
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteSecond(uint32_t second)
{
  uint32_t flag, second_buf = 0;

  /* 检查输入参数 */
  if (second >= 60)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (second >= 40)
  {
    second_buf |= (uint32_t)1uL << 6;
    second -= 40;
  }
  else
  {
    second_buf &= ~(1uL << 6);
  }

  if (second >= 20)
  {
    second_buf |= 1uL << 5;
    second -= 20;
  }
  else
  {
    second_buf &= ~(1uL << 5);
  }

  if (second >= 10)
  {
    second_buf |= 1uL << 4;
    second -= 10;
  }
  else
  {
    second_buf &= ~(1uL << 4);
  }

  if (second >= 8)
  {
    second_buf |= (1uL << 3);
    second -= 8;
  }
  else
  {
    second_buf &= ~(1uL << 3);
  }

  if (second >= 4)
  {
    second_buf |= (1uL << 2);
    second -= 4;
  }
  else
  {
    second_buf &= ~(1uL << 2);
  }

  if (second >= 2)
  {
    second_buf |= 1uL << 1;
    second -= 2;
  }
  else
  {
    second_buf &= ~(1uL << 1);
  }

  if (second >= 1)
  {
    second_buf |= 1uL;
  }
  else
  {
    second_buf &= ~1uL;
  }

  RTC->HMS.SEC = second_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteMinute
  描  述：修改分钟
  输入值：minute: 分钟
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteMinute(uint32_t minute)
{
  uint32_t flag, minute_buf = 0;

  /* 检查输入参数 */
  if (minute >= 60)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (minute >= 40)
  {
    minute_buf |= 1uL << 6;
    minute -= 40;
  }
  else
  {
    minute_buf &= ~(1uL << 6);
  }

  if (minute >= 20)
  {
    minute_buf |= 1uL << 5;
    minute -= 20;
  }
  else
  {
    minute_buf &= ~(1uL << 5);
  }

  if (minute >= 10)
  {
    minute_buf |= 1uL << 4;
    minute -= 10;
  }
  else
  {
    minute_buf &= ~(1uL << 4);
  }

  if (minute >= 8)
  {
    minute_buf |= 1uL << 3;
    minute -= 8;
  }
  else
  {
    minute_buf &= ~(1uL << 3);
  }

  if (minute >= 4)
  {
    minute_buf |= 1uL << 2;
    minute -= 4;
  }
  else
  {
    minute_buf &= ~(1uL << 2);
  }

  if (minute >= 2)
  {
    minute_buf |= 1uL << 1;
    minute -= 2;
  }
  else
  {
    minute_buf &= ~(1uL << 1);
  }

  if (minute >= 1)
  {
    minute_buf |= 1uL;
  }
  else
  {
    minute_buf &= ~1uL;
  }

  RTC->HMS.MIN = minute_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteHour
  描  述：修改小时
  输入值：hour: 小时
          meridiem：(仅在12小时模式时有效)
                    0 AM
                    1 PM
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteHour(uint32_t hour, uint32_t meridiem)
{
  uint32_t flag, hour_buf = 0, mode;

  /* 检查输入参数 */
  if (hour >= 24)
    return ERROR;

  mode = RTC_ReadHourmode();

  if (mode == RTC_HOUR12 && hour > 12)    //12小时模式不可大于12小时
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (mode == RTC_HOUR24)
  {
    if (hour >= 20)
    {
      hour_buf |= 1uL << 5;
      hour -= 20;
    }
    else
    {
      hour_buf &= ~(1uL << 5);
    }
  }
  else
  {
    if (meridiem == 1)
      hour_buf |= 1uL << 5;
    else
      hour_buf &= ~(1uL << 5);
  }

  if (hour >= 10)
  {
    hour_buf |= 1uL << 4;
    hour -= 10;
  }
  else
  {
    hour_buf &= ~(1uL << 4);
  }

  if (hour >= 8)
  {
    hour_buf |= 1uL << 3;
    hour -= 8;
  }
  else
  {
    hour_buf &= ~(1uL << 3);
  }

  if (hour >= 4)
  {
    hour_buf |= 1uL << 2;
    hour -= 4;
  }
  else
  {
    hour_buf &= ~(1uL << 2);
  }

  if (hour >= 2)
  {
    hour_buf |= 1uL << 1;
    hour -= 2;
  }
  else
  {
    hour_buf &= ~(1uL << 1);
  }

  if (hour >= 1)
  {
    hour_buf |= 1uL;
  }
  else
  {
    hour_buf &= ~1uL;
  }

  RTC->HMS.HOUR = hour_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteDay
  描  述：修改日
  输入值：day: 日
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteDay(uint32_t day)
{
  uint32_t flag, day_buf = 0;

  if ((day == 0) || (day >= 32))
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (day >= 20)
  {
    day_buf |= 1uL << 5;
    day -= 20;
  }
  else
  {
    day_buf &= ~(1uL << 5);
  }

  if (day >= 10)
  {
    day_buf |= 1uL << 4;
    day -= 10;
  }
  else
  {
    day_buf &= ~(1uL << 4);
  }

  if (day >= 8)
  {
    day_buf |= 1uL << 3;
    day -= 8;
  }
  else
  {
    day_buf &= ~(1uL << 3);
  }

  if (day >= 4)
  {
    day_buf |= 1uL << 2;
    day -= 4;
  }
  else
  {
    day_buf &= ~(1uL << 2);
  }

  if (day >= 2)
  {
    day_buf |= 1uL << 1;
    day -= 2;
  }
  else
  {
    day_buf &= ~(1uL << 1);
  }

  if (day >= 1)
  {
    day_buf |= 1uL;
  }
  else
  {
    day_buf &= ~1uL;
  }

  RTC->YMDW.DAY = day_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteMonth
  描  述：修改月
  输入值：month: 月
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteMonth(uint32_t month)
{
  uint32_t flag, month_buf = 0;

  /* 检查输入参数 */
  if ((month == 0) || (month >= 13))
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (month >= 10)
  {
    month_buf |= 1uL << 4;
    month -= 10;
  }
  else
  {
    month_buf &= ~(1uL << 4);
  }

  if (month >= 8)
  {
    month_buf |= 1uL << 3;
    month -= 8;
  }
  else
  {
    month_buf &= ~(1uL << 3);
  }

  if (month >= 4)
  {
    month_buf |= 1uL << 2;
    month -= 4;
  }
  else
  {
    month_buf &= ~(1uL << 2);
  }

  if (month >= 2)
  {
    month_buf |= 1uL << 1;
    month -= 2;
  }
  else
  {
    month_buf &= ~(1uL << 1);
  }

  if (month >= 1)
  {
    month_buf |= 1uL;
  }
  else
  {
    month_buf &= ~1uL;
  }

  RTC->YMDW.MON = month_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteYear
  描  述：修改年
  输入值：year: 年
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteYear(uint32_t year)
{
  uint32_t flag, year_buf = 0;

  /* 检查输入参数 */
  if ((year < 2000) || (year > 2099))
    return ERROR;

  /* 年转换 */
  year -= 2000;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (year >= 80)
  {
    year_buf |= 1uL << 7;
    year -= 80;
  }
  else
  {
    year_buf &= ~(1uL << 7);
  }

  if (year >= 40)
  {
    year_buf |= 1uL << 6;
    year -= 40;
  }
  else
  {
    year_buf &= ~(1uL << 6);
  }

  if (year >= 20)
  {
    year_buf |= 1uL << 5;
    year -= 20;
  }
  else
  {
    year_buf &= ~(1uL << 5);
  }

  if (year >= 10)
  {
    year_buf |= 1uL << 4;
    year -= 10;
  }
  else
  {
    year_buf &= ~(1uL << 4);
  }

  if (year >= 8)
  {
    year_buf |= 1uL << 3;
    year -= 8;
  }
  else
  {
    year_buf &= ~(1uL << 3);
  }

  if (year >= 4)
  {
    year_buf |= 1uL << 2;
    year -= 4;
  }
  else
  {
    year_buf &= ~(1uL << 2);
  }

  if (year >= 2)
  {
    year_buf |= 1uL << 1;
    year -= 2;
  }
  else
  {
    year_buf &= ~(1uL << 1);
  }

  if (year >= 1)
  {
    year_buf |= 1uL;
  }
  else
  {
    year_buf &= ~1uL;
  }

  RTC->YMDW.YEAR = year_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteWeek
  描  述：修改星期
  输入值：week: 星期（0-6）
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteWeek(uint32_t week)
{
  uint32_t flag, week_buf = 0;

  /* 检查输入参数 */
  if (week > 6)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (week >= 6)
  {
    week_buf |= 1uL << 6;
    week -= 6;
  }
  else
  {
    week_buf &= ~(1uL << 6);
  }

  if (week >= 5)
  {
    week_buf |= 1uL << 5;
    week -= 5;
  }
  else
  {
    week_buf &= ~(1uL << 5);
  }

  if (week >= 4)
  {
    week_buf |= 1uL << 4;
    week -= 4;
  }
  else
  {
    week_buf &= ~(1uL << 4);
  }

  if (week >= 3)
  {
    week_buf |= 1uL << 3;
    week -= 3;
  }
  else
  {
    week_buf &= ~(1uL << 3);
  }

  if (week >= 2)
  {
    week_buf |= 1uL << 2;
    week -= 2;
  }
  else
  {
    week_buf &= ~(1uL << 2);
  }

  if (week >= 1)
  {
    week_buf |= 1uL << 1;
  }
  else
  {
    week_buf &= ~(1uL << 1);
  }

  RTC->YMDW.WEAK = week_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_ReadWeekAlarmMinute
  描  述：读取分钟
  输入值：无
  输出值：无
  返回值：当前的分钟
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmMinute(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->WA.WM & (uint32_t)0x01) * 1);
  result += (((RTC->WA.WM >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->WA.WM >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->WA.WM >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->WA.WM >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->WA.WM >> 5) & (uint32_t)0x01) * 20);
  result += (((RTC->WA.WM >> 6) & (uint32_t)0x01) * 40);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadWeekAlarmHour
  描  述：读取小时
  输入值：无
  输出值：无
  返回值：当前的小时
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmHour(uint32_t *meridiem)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->WA.WH & (uint32_t)0x01) * 1);
  result += (((RTC->WA.WH >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->WA.WH >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->WA.WH >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->WA.WH >> 4) & (uint32_t)0x01) * 10);

  if (RTC_ReadHourmode() == RTC_HOUR24)                       //24小时模式：20小时
  {
    result += (((RTC->WA.WH >> 5) & (uint32_t)0x01) * 20);
  }
  else                                                        //12小时模式：1 PM,0 AM
  {
    *meridiem = (((RTC->WA.WH >> 5) & (uint32_t)0x01));
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadWeekAlarmWeek
  描  述：读取星期
  输入值：无
  输出值：无
  返回值：当前的星期
 ***************************************************************/
uint32_t RTC_ReadWeekAlarmWeek(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  switch (RTC->WA.WW)
  {
    case 0x01:
      result = 0;
      break;

    case 0x02:
      result = 1;
      break;

    case 0x04:
      result = 2;
      break;

    case 0x08:
      result = 3;
      break;

    case 0x10:
      result = 4;
      break;

    case 0x20:
      result = 5;
      break;

    case 0x40:
      result = 6;
      break;

    default:
      result = 0;
      break;
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadDayAlarmMinute
  描  述：读取分钟
  输入值：无
  输出值：无
  返回值：当前的分钟
 ***************************************************************/
uint32_t RTC_ReadDayAlarmMinute(void)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->DA.DM & (uint32_t)0x01) * 1);
  result += (((RTC->DA.DM >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->DA.DM >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->DA.DM >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->DA.DM >> 4) & (uint32_t)0x01) * 10);
  result += (((RTC->DA.DM >> 5) & (uint32_t)0x01) * 20);
  result += (((RTC->DA.DM >> 6) & (uint32_t)0x01) * 40);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_ReadDaykAlarmHour
  描  述：读取小时
  输入值：无
  输出值：无
  返回值：当前的小时
 ***************************************************************/
uint32_t RTC_ReadDayAlarmHour(uint32_t *meridiem)
{
  uint32_t flag;
  uint32_t result;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 触发读操作步骤 */
  RTC_StartRead();

  /* 计算时间 */
  result = ((RTC->DA.DH & (uint32_t)0x01) * 1);
  result += (((RTC->DA.DH >> 1) & (uint32_t)0x01) * 2);
  result += (((RTC->DA.DH >> 2) & (uint32_t)0x01) * 4);
  result += (((RTC->DA.DH >> 3) & (uint32_t)0x01) * 8);
  result += (((RTC->DA.DH >> 4) & (uint32_t)0x01) * 10);

  if (RTC_ReadHourmode() == RTC_HOUR24)                       //24小时模式：20小时
  {
    result += (((RTC->DA.DH >> 5) & (uint32_t)0x01) * 20);
  }
  else                                                        //12小时模式：1 PM,0 AM
  {
    *meridiem = (((RTC->DA.DH >> 5) & (uint32_t)0x01));
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return result;
}

/***************************************************************
  函数名：RTC_WriteWeekAlarmMinute
  描  述：修改分钟
  输入值：minute: 分钟
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmMinute(uint32_t minute)
{
  uint32_t flag, minute_buf = 0;

  /* 检查输入参数 */
  if (minute >= 60)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (minute >= 40)
  {
    minute_buf |= 1uL << 6;
    minute -= 40;
  }
  else
  {
    minute_buf &= ~(1uL << 6);
  }

  if (minute >= 20)
  {
    minute_buf |= 1uL << 5;
    minute -= 20;
  }
  else
  {
    minute_buf &= ~(1uL << 5);
  }

  if (minute >= 10)
  {
    minute_buf |= 1uL << 4;
    minute -= 10;
  }
  else
  {
    minute_buf &= ~(1uL << 4);
  }

  if (minute >= 8)
  {
    minute_buf |= 1uL << 3;
    minute -= 8;
  }
  else
  {
    minute_buf &= ~(1uL << 3);
  }

  if (minute >= 4)
  {
    minute_buf |= 1uL << 2;
    minute -= 4;
  }
  else
  {
    minute_buf &= ~(1uL << 2);
  }

  if (minute >= 2)
  {
    minute_buf |= 1uL << 1;
    minute -= 2;
  }
  else
  {
    minute_buf &= ~(1uL << 1);
  }

  if (minute >= 1)
  {
    minute_buf |= 1uL;
  }
  else
  {
    minute_buf &= ~1uL;
  }

  RTC->WA.WM = minute_buf;

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteWeekAlarmHour
  描  述：修改小时
  输入值：hour: 小时
          meridiem：(仅在12小时模式时有效)
                    0 AM
                    1 PM
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmHour(uint32_t hour, uint32_t meridiem)
{
  uint32_t flag, hour_buf = 0, mode;

  /* 检查输入参数 */
  if (hour >= 24)
    return ERROR;

  mode = RTC_ReadHourmode();

  if (mode == RTC_HOUR12 && hour > 12)    //12小时模式不可大于12小时
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (mode == RTC_HOUR24)
  {
    if (hour >= 20)
    {
      hour_buf |= 1uL << 5;
      hour -= 20;
    }
    else
    {
      hour_buf &= ~(1uL << 5);
    }
  }
  else
  {
    if (meridiem == 1)
      hour_buf |= 1uL << 5;
    else
      hour_buf &= ~(1uL << 5);
  }

  if (hour >= 10)
  {
    hour_buf |= 1uL << 4;
    hour -= 10;
  }
  else
  {
    hour_buf &= ~(1uL << 4);
  }

  if (hour >= 8)
  {
    hour_buf |= 1uL << 3;
    hour -= 8;
  }
  else
  {
    hour_buf &= ~(1uL << 3);
  }

  if (hour >= 4)
  {
    hour_buf |= 1uL << 2;
    hour -= 4;
  }
  else
  {
    hour_buf &= ~(1uL << 2);
  }

  if (hour >= 2)
  {
    hour_buf |= 1uL << 1;
    hour -= 2;
  }
  else
  {
    hour_buf &= ~(1uL << 1);
  }

  if (hour >= 1)
  {
    hour_buf |= 1uL;
  }
  else
  {
    hour_buf &= ~1uL;
  }

  RTC->WA.WH = hour_buf;


  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteWeekAlarmWeek
  描  述：修改星期
  输入值：week: 星期（0-6）
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteWeekAlarmWeek(uint32_t week)
{
  uint32_t flag, week_buf = 0;

  /* 检查输入参数 */
  if (week > 6)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (week >= 6)
  {
    week_buf |= 1uL << 6;
    week -= 6;
  }
  else
  {
    week_buf &= ~(1uL << 6);
  }

  if (week >= 5)
  {
    week_buf |= 1uL << 5;
    week -= 5;
  }
  else
  {
    week_buf &= ~(1uL << 5);
  }

  if (week >= 4)
  {
    week_buf |= 1uL << 4;
    week -= 4;
  }
  else
  {
    week_buf &= ~(1uL << 4);
  }

  if (week >= 3)
  {
    week_buf |= (1uL << 3);
    week -= 3;
  }
  else
  {
    week_buf &= ~(1uL << 3);
  }

  if (week >= 2)
  {
    week_buf |= 1uL << 2;
    week -= 2;
  }
  else
  {
    week_buf &= ~(1uL << 2);
  }

  if (week >= 1)
  {
    week_buf |= 1uL << 1;
  }
  else
  {
    week_buf &= ~(1uL << 1);
  }

  RTC->WA.WW = week_buf;

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteDayAlarmMinute
  描  述：修改分钟
  输入值：minute: 分钟
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteDayAlarmMinute(uint32_t minute)
{
  uint32_t flag, minute_buf = 0;

  /* 检查输入参数 */
  if (minute >= 60)
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (minute >= 40)
  {
    minute_buf |= 1uL << 6;
    minute -= 40;
  }
  else
  {
    minute_buf &= ~(1uL << 6);
  }

  if (minute >= 20)
  {
    minute_buf |= 1uL << 5;
    minute -= 20;
  }
  else
  {
    minute_buf &= ~(1uL << 5);
  }

  if (minute >= 10)
  {
    minute_buf |= 1uL << 4;
    minute -= 10;
  }
  else
  {
    minute_buf &= ~(1uL << 4);
  }

  if (minute >= 8)
  {
    minute_buf |= 1uL << 3;
    minute -= 8;
  }
  else
  {
    minute_buf &= ~(1uL << 3);
  }

  if (minute >= 4)
  {
    minute_buf |= 1uL << 2;
    minute -= 4;
  }
  else
  {
    minute_buf &= ~(1uL << 2);
  }

  if (minute >= 2)
  {
    minute_buf |= 1uL << 1;
    minute -= 2;
  }
  else
  {
    minute_buf &= ~(1uL << 1);
  }

  if (minute >= 1)
  {
    minute_buf |= 1uL;
  }
  else
  {
    minute_buf &= ~1uL;
  }

  RTC->DA.DM = minute_buf;

  /* 触发写操作步骤 */
  RTC->CON.TMWR = 1;      // 操作类型是写
  RTC->CON.TMUP = 1;      // 触发写操作

  while (RTC->CON.TMUP == 1);

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_WriteDayAlarmHour
  描  述：修改小时
  输入值：hour: 小时
          meridiem：(仅在12小时模式时有效)
                    0 AM
                    1 PM
  输出值：无
  返回值：无
 ***************************************************************/
ErrorStatus RTC_WriteDayAlarmHour(uint32_t hour, uint32_t meridiem)
{
  uint32_t flag, hour_buf = 0, mode;

  /* 检查输入参数 */
  if (hour >= 24)
    return ERROR;

  mode = RTC_ReadHourmode();

  if (mode == RTC_HOUR12 && hour > 12)    //12小时模式不可大于12小时
    return ERROR;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  if (mode == RTC_HOUR24)
  {
    if (hour >= 20)
    {
      hour_buf |= 1uL << 5;
      hour -= 20;
    }
    else
    {
      hour_buf &= ~(1uL << 5);
    }
  }
  else
  {
    if (meridiem == 1)
      hour_buf |= 1uL << 5;
    else
      hour_buf &= ~(1uL << 5);
  }

  if (hour >= 10)
  {
    hour_buf |= 1uL << 4;
    hour -= 10;
  }
  else
  {
    hour_buf &= ~(1uL << 4);
  }

  if (hour >= 8)
  {
    hour_buf |= 1uL << 3;
    hour -= 8;
  }
  else
  {
    hour_buf &= ~(1uL << 3);
  }

  if (hour >= 4)
  {
    hour_buf |= 1uL << 2;
    hour -= 4;
  }
  else
  {
    hour_buf &= ~(1uL << 2);
  }

  if (hour >= 2)
  {
    hour_buf |= 1uL << 1;
    hour -= 2;
  }
  else
  {
    hour_buf &= ~(1uL << 1);
  }

  if (hour >= 1)
  {
    hour_buf |= 1uL;
  }
  else
  {
    hour_buf &= ~1uL;
  }

  RTC->DA.DH = hour_buf;



  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return SUCCESS;
}

/***************************************************************
  函数名：RTC_InterruptEnable
  描  述：使能实时时钟的某些中断
  输入值：src: 实时时钟的中断源
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_InterruptEnable(RTC_Interrupt_Source src)
{
  uint32_t flag;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  switch (src)
  {
    case RTC_Interrupt_Source_Second:
      RTC->IE.SCDIE = 1;
      break;

    case RTC_Interrupt_Source_Minute:
      RTC->IE.MINIE = 1;
      break;

    case RTC_Interrupt_Source_Hour:
      RTC->IE.HORIE = 1;
      break;

    case RTC_Interrupt_Source_Day:
      RTC->IE.DAYIE = 1;
      break;

    case RTC_Interrupt_Source_Month:
      RTC->IE.MONIE = 1;
      break;

    case RTC_Interrupt_Source_DayALE:
      RTC->IE.DALE = 1;
      break;

    case RTC_Interrupt_Source_WeekALE:
      RTC->IE.WALE = 1;
      break;

    default:
      break;
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  NVIC->ISER[0] = (1uL << 17);  // 使能NVIC中断
  return;
}

/***************************************************************
  函数名：RTC_InterruptDisable
  描  述：禁能实时时钟的某些中断
  输入值：src: 实时时钟的中断源
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_InterruptDisable(RTC_Interrupt_Source src)
{
  uint32_t flag;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
  switch (src)
  {
    case RTC_Interrupt_Source_Second:
      RTC->IE.SCDIE = 0;
      break;

    case RTC_Interrupt_Source_Minute:
      RTC->IE.MINIE = 0;
      break;

    case RTC_Interrupt_Source_Hour:
      RTC->IE.HORIE = 0;
      break;

    case RTC_Interrupt_Source_Day:
      RTC->IE.DAYIE = 0;
      break;

    case RTC_Interrupt_Source_Month:
      RTC->IE.MONIE = 0;
      break;

    case RTC_Interrupt_Source_DayALE:
      RTC->IE.DALE = 0;
      break;

    case RTC_Interrupt_Source_WeekALE:
      RTC->IE.WALE = 0;
      break;

    default:
      break;
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return;
}

/***************************************************************
  函数名：RTC_GetITStatus
  描  述：读取实时时钟的某些中断状态
  输入值：src: 实时时钟的中断源
  输出值：无
  返回值：中断标志
 ***************************************************************/
ITStatus RTC_GetITStatus(RTC_Interrupt_Source src)
{
  ITStatus result = RESET;

  switch (src)
  {
    case RTC_Interrupt_Source_Second:
      if (RTC->IE.SCDIE)
        result = SET;

      break;

    case RTC_Interrupt_Source_Minute:
      if (RTC->IE.MINIE)
        result = SET;

      break;

    case RTC_Interrupt_Source_Hour:
      if (RTC->IE.HORIE)
        result = SET;

      break;

    case RTC_Interrupt_Source_Day:
      if (RTC->IE.DAYIE)
        result = SET;

      break;

    case RTC_Interrupt_Source_Month:
      if (RTC->IE.MONIE)
        result = SET;

      break;

    case RTC_Interrupt_Source_DayALE:
      if (RTC->IE.DALE)
        result = SET;

      break;

    case RTC_Interrupt_Source_WeekALE:
      if (RTC->IE.WALE)
        result = SET;

      break;

    default:
      result = RESET;
      break;
  }

  return result;
}

/***************************************************************
  函数名：RTC_GetITFlag
  描  述：读取实时时钟的某些中断标志
  输入值：src: 实时时钟的中断源
  输出值：无
  返回值：中断标志
 ***************************************************************/
FlagStatus RTC_GetFlagStatus(RTC_Interrupt_Source src)
{
  FlagStatus result = RESET;

  switch (src)
  {
    case RTC_Interrupt_Source_Second:
      if (RTC->IF.SCDIF)
        result = SET;

      break;

    case RTC_Interrupt_Source_Minute:
      if (RTC->IF.MINIF)
        result = SET;

      break;

    case RTC_Interrupt_Source_Hour:
      if (RTC->IF.HORIF)
        result = SET;

      break;

    case RTC_Interrupt_Source_Day:
      if (RTC->IF.DAYIF)
        result = SET;

      break;

    case RTC_Interrupt_Source_Month:
      if (RTC->IF.MONIF)
        result = SET;

      break;

    case RTC_Interrupt_Source_DayALE:
      if (RTC->IF.DAFG)
        result = SET;

      break;

    case RTC_Interrupt_Source_WeekALE:
      if (RTC->IF.WAFG)
        result = SET;

      break;

    default:
      result = RESET;
      break;
  }

  return result;
}


/***************************************************************
  函数名：RTC_ClearITPendingBit
  描  述：清除指定的中断标志位
  输入值：src: 实时时钟的中断源
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_ClearITPendingBit(RTC_Interrupt_Source src)
{
  uint32_t flag;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  switch (src)
  {
    case RTC_Interrupt_Source_Second:
      RTC->IF.Word = RTC_Interrupt_Source_Second;
      break;

    case RTC_Interrupt_Source_Minute:
      RTC->IF.Word = RTC_Interrupt_Source_Minute;
      break;

    case RTC_Interrupt_Source_Hour:
      RTC->IF.Word = RTC_Interrupt_Source_Hour;
      break;

    case RTC_Interrupt_Source_Day:
      RTC->IF.Word = RTC_Interrupt_Source_Day;
      break;

    case RTC_Interrupt_Source_Month:
      RTC->IF.Word = RTC_Interrupt_Source_Month;
      break;

    case RTC_Interrupt_Source_DayALE:
      RTC->IF.Word = RTC_Interrupt_Source_DayALE;
      break;

    case RTC_Interrupt_Source_WeekALE:
      RTC->IF.Word = RTC_Interrupt_Source_WeekALE;
      break;

    default:

      break;
  }

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return;
}

/***************************************************************
  函数名：RTC_ClearAllITFlag
  描  述：清除实时时钟的所有中断标志
  输入值：无
  输出值：无
  返回值：无
 ***************************************************************/
void RTC_ClearAllITFlag(void)
{
  uint32_t flag;

  /* 解除RTC写保护 */
  flag = RTC->WP.WP;

  if (flag == 0x00000000)
    RTC->WP.WP = 0x55AAAA55;

  /* 设置新的值 */
//    RTC->IF.SCDIF = 1;  // 写1清除中断标志位
//    RTC->IF.MINIF = 1;  // 写1清除中断标志位
//    RTC->IF.HORIF = 1;  // 写1清除中断标志位
//    RTC->IF.DAYIF = 1;  // 写1清除中断标志位
//    RTC->IF.MONIF = 1;  // 写1清除中断标志位
//    RTC->IF.DAFG = 1;  // 写1清除中断标志位
//    RTC->IF.WAFG = 1;  // 写1清除中断标志位
  RTC->IF.Word = 0xFFFFFFFF;

  /* 加入RTC写保护 */
  if (flag == 0x00000000)
    RTC->WP.WP = 0x00000000;

  return;
}
