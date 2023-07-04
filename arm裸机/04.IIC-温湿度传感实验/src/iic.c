#include "iic.h"

extern void printf(const char *fmt, ...);
/*
 * 函数名 ： delay_us
 * 函数功能：延时函数
 * 函数参数：无
 * 函数返回值：无
 * */
void delay_us(void)
{
    unsigned int i = 2000;
    while (i--)
        ;
}
/*
 * 函数名 ： i2c_init
 * 函数功能： i2C总线引脚的初始化， 通用输出，推挽输出，输出速度，
 * 函数参数：无
 * 函数返回值：无
 * */
void i2c_init(void)
{
    // 使能GPIOF端口的时钟
    RCC->MP_AHB4ENSETR |= (0x1 << 5);
    // 设置PF14，PF15引脚为通用的输出功能
    GPIOF->MODER &= (~(0xF << 28));
    GPIOF->MODER |= (0x5 << 28);
    // 设置PF14, PF15引脚为推挽输出
    GPIOF->OTYPER &= (~(0x3 << 14));
    // 设置PF14, PF15引脚为高速输出
    GPIOF->OSPEEDR |= (0xF << 28);
    // 设置PF14, PF15引脚的禁止上拉和下拉
    GPIOF->PUPDR &= (~(0xF << 28));
    // 空闲状态SDA和SCL拉高
    I2C_SCL_H;
    I2C_SDA_H;
}

/*
 * 函数名：i2c_start
 * 函数功能：模拟i2c开始信号的时序
 * 函数参数：无
 * 函数返回值：无
 * */
void i2c_start(void)
{
    /*
     * 开始信号：时钟在高电平期间，数据线从高到低的变化
     *     --------
     * SCL         \
     *              --------
     *     ----
     * SDA     \
     *          --------
     * */
    SET_SDA_OUT; // 设置数据线为输出模式
    I2C_SCL_H; // SCL拉高
    delay_us();
    I2C_SDA_H; // SDA拉高
    delay_us();
    I2C_SDA_L; // SDA拉低
    I2C_SCL_L; // 起始信号产生之后，总线处于占用状态
}

/*
 * 函数名：i2c_stop
 * 函数功能：模拟i2c停止信号的时序
 * 函数参数：无
 * 函数返回值：无
 * */

void i2c_stop(void)
{
    /*
     * 停止信号 ： 时钟在高电平期间，数据线从低到高的变化
     *             ----------
     * SCL        /
     *    --------
     *    ---         -------
     * SDA   X       /
     *    --- -------
     * */
    SET_SDA_OUT; // 1.设置数据线为输出模式
    I2C_SCL_L; // 2.SCL拉低，数据线上数据允许发生变化
    delay_us();
    I2C_SDA_L; // 3.SDA拉低
    delay_us();
    I2C_SCL_H; // 4.SCL拉高
    delay_us();
    I2C_SDA_H; // 5.SDA拉高
    delay_us();
}

/*
 * 函数名： i2c_write_byte
 * 函数功能：主机向i2c总线上的从设备写8bits数据
 * 函数参数：dat : 等待发送的字节数据
 * 函数返回值： 无
 * */
void i2c_write_byte(unsigned char dat)
{
    /*
     * 数据信号：时钟在低电平期间，发送器向数据线上写入数据
     * 			时钟在高电平期间，接收器从数据线上读取数据
     *      ----          --------
     * 	SCL     \        /        \
     *           --------          --------
     *      -------- ------------------ ---
     * 	SDA         X                  X
     *      -------- ------------------ ---
     *
     *      先发送高位在发送低位
     * */
    unsigned int i;
    SET_SDA_OUT; // 1.设置总线为输出模式
    // 2.for循环 条件8位 实现 低电平期间，高 低 移位
    for (i = 0; i < 8; i++) {
        I2C_SCL_L; // 3.SCL拉低,向数据线上写入数据
        delay_us();
        if (dat & 0x80) // 4.先发送高位在发送低位
            I2C_SDA_H; // 5.向数据线上写入高电平
        else
            I2C_SDA_L; // 6.向数据线上写入低电平
        delay_us();
        I2C_SCL_H; // 7.等待从机从总线上读取数据
        delay_us();
        delay_us();
        dat <<= 1; // 发送下一位
    }
}

/*
 * 函数名：i2c_read_byte
 * 函数功能： 主机从i2c总线上的从设备读8bits数据,
 *          主机发送一个应答或者非应答信号
 * 函数参数： 0 ： 应答信号   1 ： 非应答信号
 * 函数返回值：读到的有效数据
 *
 * */
unsigned char i2c_read_byte(unsigned char ack)
{
    /*
     * 数据信号：时钟在低电平期间，发送器向数据线上写入数据
     * 			时钟在高电平期间，接收器从数据线上读取数据
     *      ----          --------
     * 	SCL     \        /        \
     *           --------          --------
     *      -------- ------------------ ---
     * 	SDA         X                  X
     *      -------- ------------------ ---
     *
     *      先接收高位， 在接收低位
     * */
    unsigned int i;
    unsigned char dat; // 存放读取的值
    SET_SDA_IN; // 1.设置数据线为输入模式
    // 2.for循环 条件  实现 scl高电平期间
    for (i = 0; i < 8; i++) {
        I2C_SCL_L; // 3.SCL拉低，等待从机从总线上写完数据
        delay_us();
        delay_us();

        I2C_SCL_H; // 4.SCL拉高,从数据线上读取数据
        delay_us();
        dat <<= 1; // 5.移位必须放在这个地方!!!
        if (I2C_SDA_READ) // 6.从总线上读取的内容
            dat |= 1; // 7.读到的是高电平
        else
            dat |= 0; // 8.读到的是低电平
        delay_us();
    }
    if (!ack)
        i2c_ack(); // 9.主机给从机应答信号
    else
        i2c_nack(); // 10.主机给从机非应答信号
    return dat; // 返回读取到的值
}
/*
 * 函数名： i2c_wait_ack
 * 函数功能： 主机作为发送器时，等待接收器返回的应答信号
 * 函数参数：无
 * 函数返回值：
 *					0：接收到的应答信号
 *                  1：接收到的非应答信号
 * */
unsigned char i2c_wait_ack(void)
{
    /*
     * 主机发送一个字节之后，从机给主机返回一个应答信号
     *
     *                   -----------
     * SCL              /   M:读    \
     *     -------------             --------
     *     --- ---- --------------------
     * SDA    X    X
     *     ---      --------------------
     *     主  释   从机    主机
     *     机  放   向数据  读数据线
     *         总   线写    上的数据
     *         线   数据
     * */

    I2C_SCL_L; // 1.SCL拉低
    delay_us();
    I2C_SDA_H; // 2.SDA拉高，释放总线，总线处于空闲状态
    delay_us();
    SET_SDA_IN; // 3.设置数据线为输入模式
    delay_us();
    I2C_SCL_H; // 4.SCL拉高，从总线上读取的内容
    delay_us();
    if (I2C_SDA_READ)
        return 1; // 非应答信号
    I2C_SCL_L; // 5.总线处于占用状态
    return 0; // 6.应答信号
}

/*
 * 函数名： iic_ack
 * 函数功能： 主机作为接收器时，给发送器发送应答信号
 * 函数参数：无
 * 函数返回值：无
 * */
void i2c_ack(void)
{
    /*            --------
     * SCL       /        \
     *    -------          ------
     *    ---
     * SDA   X
     *    --- -------------
     * */
    SET_SDA_OUT; // 1.设置数据线为输出模式
    I2C_SCL_L; // 2.SCL拉低，改变数据线上数据
    delay_us();
    I2C_SDA_L; // 3.SDA拉低，应答信号为0
    delay_us();
    I2C_SCL_H; // 4.SCL拉高，从总线上读取的内容
    delay_us();
    delay_us();
    I2C_SCL_L; // 5.SCL拉低，总线处于占用状态
}
/*
 * 函数名： iic_nack
 * 函数功能： 主机作为接收器时，给发送器发送非应答信号
 * 函数参数：无
 * 函数返回值：无
 * */
void i2c_nack(void)
{
    /*            --------
     * SCL       /        \
     *    -------          ------
     *    --- ---------------
     * SDA   X
     *    ---
     * */
    SET_SDA_OUT; // 1.设置数据线为输出模式
    I2C_SCL_L; // 2.SCL拉低，改变数据线上数据
    delay_us();
    I2C_SDA_H; // 3.SDA拉高，非应答信号为1
    delay_us();
    I2C_SCL_H; // 4.SCL拉高，从总线上读取的内容
    delay_us();
    delay_us();
    I2C_SCL_L; // 5.SCL拉低，总线处于占用状态
}
