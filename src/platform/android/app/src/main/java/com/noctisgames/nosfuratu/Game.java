package com.noctisgames.nosfuratu;

import android.content.res.AssetManager;

public final class Game
{
    static
    {
        System.loadLibrary("game");
    }

    public static native void init(AssetManager assetManager, boolean isLowMemoryDevice);

    public static native void on_surface_created();

    public static native void on_surface_changed(int pixelWidth, int pixelHeight);

    public static native void on_resume();

    public static native void on_pause();

    public static native void update(float deltaTime);

    public static native void render();

    public static native void on_touch_down(float normalized_x, float normalized_y);

    public static native void on_touch_dragged(float normalized_x, float normalized_y);

    public static native void on_touch_up(float normalized_x, float normalized_y);

    public static native short get_current_music_id();

    public static native short get_current_sound_id();

    public static native int get_requested_action();

    public static native void clear_requested_action();

    public static native boolean handle_on_back_pressed();

    public static native void load_user_save_data(String level_json);

    public static native int get_score();

    public static native int get_online_score();

    public static native int get_level_stats_flag();

    public static native int get_num_golden_carrots();

    public static native int get_jon_unlocked_abilities_flag();

    public static native int get_level_stats_flag_for_unlocked_level();

    public static native int get_num_golden_carrots_after_unlocking_level();
}