# Tiva C Servo Control with FreeRTOS

This repository contains a FreeRTOS-based implementation for controlling a standard servo motor using the TM4C123GXL (Tiva C Series) LaunchPad. The project uses hardware PWM and onboard push buttons to toggle the servo between 0 and 90 degrees.

## 🛠 Hardware Connections

| Servo Wire | Tiva C Pin | Function |
| :--- | :--- | :--- |
| **Red (Power)** | **VBUS** | 5V Power Supply |
| **Black (GND)** | **GND** | Ground |
| **White/Yellow**| **PD0** | PWM Signal (M0PWM6) |

## 🎮 Controls

The project utilizes the two onboard buttons (SW1 and SW2) to control the rotation:
*   **Left Button (SW1 / PF4):** Sets servo angle to **0°** (1.0ms pulse).
*   **Right Button (SW2 / PF0):** Sets servo angle to **90°** (2.0ms pulse).

## ⚙️ Technical Details

The system is configured with the following parameters to ensure precise timing:

*   **Processor Clock:** 50 MHz
*   **PWM Clock Divider:** 64
*   **PWM Frequency:** 50 Hz (20ms Period)
*   **Duty Cycle Calculations (at 50MHz):**
    *   **Period:** 15,625 ticks
    *   **0° (1.0ms):** 781 ticks
    *   **90° (2.0ms):** 1,563 ticks

## 🏗 Software Architecture

The project is built on **FreeRTOS** to ensure modularity and responsiveness:
1.  **Switch Task:** Monitors GPIO interrupts for button presses and sends messages to a queue.
2.  **Servo Task:** Waits for queue messages and updates the PWM Pulse Width registers.
3.  **Queue System:** Decouples the input (buttons) from the output (PWM) to prevent blocking.

## 🚀 Getting Started

### Prerequisites
*   [TivaWare SDK](https://www.ti.com/tool/SW-TM4C)
*   Code Composer Studio (CCS) or Arm GNU Toolchain
*   FreeRTOS Source Files

### Installation
1.  Clone the repository:
    ```bash
    git clone https://github.com/YOUR_USERNAME/Tiva_C_Servo_Control.git
    ```
2.  Import the project into your IDE.
3.  Ensure the TivaWare path is correctly linked in your project properties.
4.  Build and Flash to your LaunchPad.

## ⚠️ Important Note on Power
While the Tiva C **VBUS** pin can power small SG90 servos, larger servos or high-load applications may cause a voltage drop (brown-out) that resets the MCU. For stable operation, use an external 5V power supply and connect the grounds.

 <img width="768" height="1024" alt="Image (1)" src="https://github.com/user-attachments/assets/2f2cf54e-b787-45de-8545-31c4c58614b9" />


https://github.com/user-attachments/assets/0f0adafa-d69d-4391-b76c-2b1fea11fac7


