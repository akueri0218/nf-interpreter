

Pre-configured cmake-variants.json for this board

```
{
  "buildType": {
    "default": "debug",
    "choices": {
      "debug": {
        "short": "Debug",
        "long": "Emit debug information without performing optimizations",
        "buildType": "Debug"
      },
      "minsize": {
        "short": "MinSizeRel",
        "long": "Optimize for smallest binary size",
        "buildType": "MinSizeRel"
      },
      "reldeb": {
        "short": "RelWithDebInfo",
        "long": "Perform optimizations AND include debugging information",
        "buildType": "RelWithDebInfo"
      }
    }
  },
  "linkage": {
    "default": "",
    "choices": {
      "Maixduino": {
        "short": "Maixduino",
        "settings": {
          "BUILD_VERSION": "0.9.99.999",
          "BUILD_VERBOSE": "OFF",
          "CMAKE_TOOLCHAIN_FILE": "CMake/toolchain.riscv64-unknown-elf.cmake",
          "TOOLCHAIN_PREFIX": "<absolute-path-to-riscv-toolchain-folder-mind-the-forward-slashes>",
          "TOOL_SRECORD_PREFIX": "<absolute-path-to-srecord-folder-mind-the-forward-slashes>",
          "TARGET_SERIES": "Maix_M1",
          "SUPPORT_ANY_BASE_CONVERSION": "ON",
          "RTOS": "FreeRTOS",
          "TARGET_BOARD": "Maixduino",
          "RTOS_VERSION": "",
          "CMSIS_VERSION": "",
          "FATFS_VERSION": "",
          "LWIP_VERSION": "",
          "NF_BUILD_RTM": "OFF",
          "NF_FEATURE_DEBUGGER": "ON",
          "NF_FEATURE_HAS_SDCARD": "OFF", "TODO1": "ON",
          "NF_FEATURE_RTC": "ON",
          "NF_FEATURE_HAS_CONFIG_BLOCK": "ON",
          "API_Windows.Devices.Gpio": "ON",
          "API_System.Device.Gpio": "ON",
          "API_Windows.Devices.Spi": "OFF", "TODO2": "ON",
          "API_Windows.Devices.I2c": "ON",
          "API_System.Device.I2c": "ON",
          "API_Windows.Devices.Adc": "OFF", "TODO3": "ON",
          "API_System.Device.Adc": "OFF", "TODO4": "ON",
          "API_Windows.Devices.Pwm": "OFF", "TODO5": "ON?",
          "API_System.Device.WiFi": "OFF", "TODO6": "ON",
          "API_Windows.Devices.SerialCommunication": "ON",
          "API_System.IO.Ports": "ON",
          "API_nanoFramework.ResourceManager": "ON",
          "API_nanoFramework.System.Collections": "ON",
          "API_System.Net": "ON", "TODO7": "ON",
          "API_System.Math": "ON"
        }
      }
    }
  }
}
```

