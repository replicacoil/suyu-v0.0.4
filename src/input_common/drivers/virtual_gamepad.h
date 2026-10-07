// SPDX-FileCopyrightText: Copyright 2022 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <functional>
#include "input_common/input_engine.h"

namespace InputCommon {

/**
 * A virtual controller that is always assigned to the game input
 */
class VirtualGamepad final : public InputEngine {
public:
    enum class VirtualButton {
        ButtonA,
        ButtonB,
        ButtonX,
        ButtonY,
        StickL,
        StickR,
        TriggerL,
        TriggerR,
        TriggerZL,
        TriggerZR,
        ButtonPlus,
        ButtonMinus,
        ButtonLeft,
        ButtonUp,
        ButtonRight,
        ButtonDown,
        ButtonSL,
        ButtonSR,
        ButtonHome,
        ButtonCapture,
    };

    enum class VirtualStick {
        Left = 0,
        Right = 1,
    };

    explicit VirtualGamepad(std::string input_engine_);

    /**
     * Sets the status of all buttons bound with the key to pressed
     * @param player_index the player number that will take this action
     * @param button_id the id of the button
     * @param value indicates if the button is pressed or not
     */
    void SetButtonState(std::size_t player_index, int button_id, bool value);
    void SetButtonState(std::size_t player_index, VirtualButton button_id, bool value);

    /**
     * Sets the status of a stick to a specific player index
     * @param player_index the player number that will take this action
     * @param axis_id the id of the axis to move
     * @param x_value the position of the stick in the x axis
     * @param y_value the position of the stick in the y axis
     */
    void SetStickPosition(std::size_t player_index, int axis_id, float x_value, float y_value);
    void SetStickPosition(std::size_t player_index, VirtualStick axis_id, float x_value,
                          float y_value);

    /**
     * Sets the status of the motion sensor to a specific player index
     * @param player_index the player number that will take this action
     * @param delta_timestamp time passed since last reading
     * @param gyro_x,gyro_y,gyro_z the gyro sensor readings
     * @param accel_x,accel_y,accel_z the accelerometer reading
     */
    void SetMotionState(std::size_t player_index, u64 delta_timestamp, float gyro_x, float gyro_y,
                        float gyro_z, float accel_x, float accel_y, float accel_z);

    /// Restores all inputs into the neutral position
    void ResetControllers();

    /**
     * Registers a callback invoked whenever SetVibration() below is called.
     * This class has no knowledge of any particular embedding frontend, so
     * an embedder that needs to forward vibration requests somewhere (e.g.
     * the libretro core, forwarding to RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE)
     * has to register one explicitly - without it, vibration requests for
     * this engine silently go nowhere. Pass nullptr to clear it.
     * @param callback invoked with (player_index, low_amplitude,
     *        high_amplitude), each amplitude in the 0.0-1.0 range
     */
    void SetVibrationCallback(std::function<void(std::size_t player_index, float low_amplitude, float high_amplitude)>callback);

    /// InputEngine override - without this, SetVibration() falls through to
    /// the base class's no-op default and vibration requests for this engine
    /// are silently dropped (this was the case until this override was added
    /// - every other driver, e.g. SDLDriver, has always overridden this).
    Common::Input::DriverResult SetVibration(const PadIdentifier& identifier, const Common::Input::VibrationStatus& vibration) override;

    // Must return true or the vibration stack never calls SetVibration().
    bool IsVibrationEnabled(const PadIdentifier& identifier) override; 

private:
    /// Returns the correct identifier corresponding to the player index
    PadIdentifier GetIdentifier(std::size_t player_index) const;
    std::function<void(std::size_t player_index, float low_amplitude, float high_amplitude)>vibration_callback;
};

} // namespace InputCommon
