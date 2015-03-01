package com.LangStack.Log;

public class Logger 
{
	public static final int	LogLevelFATAL	= 0;
	public static final int	LogLevelERROR	= 1;
	public static final int	LogLevelWARN	= 2;
	public static final int	LogLevelINFO	= 3;
	public static final int	LogLevelDEBUG	= 4;
	public static final int	LogLevelVERBOSE	= 5;
	public static final int	LogLevelFULL	= 6;

	public static void f(String tag, String detail)
	{
		log(LogLevelFATAL, tag, detail);
	}
	public static void e(String tag, String detail)
	{
		log(LogLevelERROR, tag, detail);
	}
	public static void w(String tag, String detail)
	{
		log(LogLevelWARN, tag, detail);
	}
	public static void i(String tag, String detail)
	{
		log(LogLevelINFO, tag, detail);
	}
	public static void d(String tag, String detail)
	{
		log(LogLevelDEBUG, tag, detail);
	}
	public static void v(String tag, String detail)
	{
		log(LogLevelVERBOSE, tag, detail);
	}
	

	/**
	 * @brief	设置日志输出等级
	 * @param 	level		日志等级
	 */
	public static native void setLoggerLevel(int level);
	
	/**
	 * @brief	输出日志
	 * @param 	level		日志等级	
	 * @param	tag			日志标记
	 * @param 	detail		日志内容
	 */
	public static native void log(int level, String tag, String detail);
}
