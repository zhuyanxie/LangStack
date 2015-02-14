package com.LangStack.Task;

public class DeleteThreadTask implements ITask
{
    private TaskThread mThread = null;
    private String mTaskId = null;
    public DeleteThreadTask(TaskThread thread)
    {
        mThread = thread;
        mTaskId = "dekete thread : " + thread.getIndex();
    }

    @Override public void run()
    {
        mThread.release();
    }

    @Override public String getTaskId()
    {
        return mTaskId;
    }

}
