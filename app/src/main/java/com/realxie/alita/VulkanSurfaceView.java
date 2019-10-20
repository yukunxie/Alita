package com.realxie.alita;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.concurrent.atomic.AtomicBoolean;

public class VulkanSurfaceView extends SurfaceView implements Runnable, SurfaceHolder.Callback {

    public SurfaceHolder surfaceHolder;
    private Thread thread_;

    private boolean isPuased_ = false;
    private boolean isExit_   = false;

    public VulkanSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        this.surfaceHolder = this.getHolder();
        this.surfaceHolder.addCallback(this);
    }

    public void setPaused(boolean isPuased)
    {
        isPuased_ = isPuased;
    }

    public void setExit(boolean exit)
    {
        isExit_ = exit;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        MainActivity.initVulkanContextJNI(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        this.thread_ = new Thread(this);
        this.thread_.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        this.surfaceHolder.removeCallback(this);
    }

    @Override
    public void run() {
        while(!isExit_){
            if (!isPuased_)
            {
                MainActivity.nativeRenderJNI(1 / 60.0f);
            }

            try {
                Thread.sleep(1000/60);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
