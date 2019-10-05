package com.realxie.alita;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class VulkanSurfaceView extends SurfaceView implements Runnable, SurfaceHolder.Callback {

    public SurfaceHolder surfaceHolder;
    private Thread thread;

    public VulkanSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        this.surfaceHolder = this.getHolder();
        this.surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        MainActivity.initVulkanContextJNI(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        this.thread = new Thread(this);
        this.thread.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        this.surfaceHolder.removeCallback(this);
    }

    @Override
    public void run() {
        while(true){
            MainActivity.nativeRenderJNI(1 / 60.0f);
            try {
                Thread.sleep(1000/60);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
