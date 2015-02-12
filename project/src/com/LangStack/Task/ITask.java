package com.LangStack.Task;

public interface ITask
{
    /**
     * @brief       任务运行接口
     */
    public void run();
    
    /**
     * @brief       获取当前任务标记
     * @return      任务标记
     */
    public String getTaskId();
}
