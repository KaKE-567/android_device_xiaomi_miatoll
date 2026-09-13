# Unified Device Tree for Xiaomi Miatoll (LineageOS 18.1 / Android 11)

```
       _       _        _ _ 
 _ __ (_) __ _| |_ ___ | | |
| '_ \| |/ _` | __/ _ \| | |
| | | | | (_| | || (_) | | |
|_| |_|_|\__,_|\__\___/|_|_|
```

The **Xiaomi Miatoll** unified device tree supports all variants of the Qualcomm Snapdragon 720G (SM6250) platform:
* **curtana** (Redmi Note 9S / Redmi Note 9 Pro India)
* **gram** (POCO M2 Pro)
* **joyeuse** (Redmi Note 9 Pro Global / Europe with NFC)
* **excalibur** (Redmi Note 9 Pro Max India)

---

## Hardware Specifications

| Component | Specification |
| :--- | :--- |
| **SoC** | Qualcomm SM6250 Snapdragon 720G (8 nm) |
| **CPU** | Octa-core Kryo 465 (2x2.32 GHz Cortex-A76 & 6x1.80 GHz Cortex-A55) |
| **GPU** | Adreno 618 @ 750 MHz |
| **Memory** | 4GB / 6GB / 8GB LPDDR4X |
| **Storage** | 64GB / 128GB UFS 2.1 |
| **Display** | 6.67" IPS LCD, 1080 x 2400 pixels, 20:9 ratio |
| **Battery** | 5020 mAh (Li-Po), 18W / 33W Fast Charging |

---

## Custom Tuning & Enhancements

This device tree features tailored system, kernel scheduler, and UI optimizations:

1. **Aggressive EAS Gaming Optimization (Zero 1% Low Drops in BGMI / PUBG Mobile)**:
   * **`sched_upmigrate`** tuned to **`60`** (lowered from stock `95`) and **`sched_downmigrate`** to **`50`** (lowered from stock `85`) to ensure Unreal Engine 4 render and physics worker threads immediately migrate to Cortex-A76 Gold cores.
   * **`sched_group_upmigrate 75`** and **`sched_group_downmigrate 60`** prevents cluster ping-ponging.
   * **`top-app/schedtune.boost`** set to **`15`** with `prefer_idle 1`.
   * Schedutil ratelimits tuned to **`up_rate_limit_us 0`** (instantaneous ramp-up) and **`down_rate_limit_us 20000`** (holds high clocks across frame rendering intervals).
   * Conservative predicted load disabled (`sched_conservative_pl 0`) for rapid response to render bursts.

2. **Audio Buffer Starvation (XRUN) Elimination**:
   * Expanded `foreground` cpuset to `0-7` (unlocking both Big cores for `audioserver` and `vendor.audio-hal`).
   * Dedicated real-time schedtune boost (`rt/schedtune.boost 10`, `prefer_idle 1`) preventing sound crackling/buzzing under heavy multi-threaded game loads.

3. **Memory Direct Reclaim Fix**:
   * `watermark_scale_factor` set to `100` and `vfs_cache_pressure` set to `100`, allowing `kswapd` to asynchronously reclaim pages in the background without stalling foreground render threads.

4. **SurfaceFlinger & Compositor Tuning**:
   * Hardware Composer (`hwcomposer-2-4`) cpuset expanded to `0-5`.
   * SurfaceFlinger unsignaled buffer latching enabled (`debug.sf.latch_unsignaled=1`) to eliminate 1-frame fence dropouts.

---

## How to Build

### 1. Initialize the Source Repository
```bash
repo init -u https://github.com/LineageOS/android.git -b lineage-18.1
```

### 2. Add Local Manifest
Create `.repo/local_manifests/miatoll.xml`:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
    <!-- Device Trees -->
    <project name="KaKE-567/device_xiaomi_miatoll" path="device/xiaomi/miatoll" remote="github" revision="lineage-18.1" />
    <project name="KaKE-567/device_xiaomi_sm6250-common" path="device/xiaomi/sm6250-common" remote="github" revision="lineage-18.1" />

    <!-- Vendor Trees -->
    <project name="KaKE-567/vendor_xiaomi_miatoll" path="vendor/xiaomi/miatoll" remote="github" revision="lineage-18.1" />
    <project name="KaKE-567/vendor_xiaomi_sm6250-common" path="vendor/xiaomi/sm6250-common" remote="github" revision="lineage-18.1" />

    <!-- Kernel Tree -->
    <project name="KaKE-567/android_kernel_xiaomi_sm6250-apex" path="kernel/xiaomi/sm6250" remote="github" revision="apex" />

    <!-- Custom Packages (KaKe Elements UI & Settings) -->
    <remove-project name="LineageOS/android_packages_apps_Settings" />
    <project name="KaKE-567/android_packages_apps_Settings" path="packages/apps/Settings" remote="github" revision="lineage-18.1" />

    <remove-project name="LineageOS/android_packages_apps_LineageParts" />
    <project name="KaKE-567/android_packages_apps_LineageParts" path="packages/apps/LineageParts" remote="github" revision="lineage-18.1" />

    <remove-project name="LineageOS/android_packages_apps_Trebuchet" />
    <project name="KaKE-567/android_packages_apps_Trebuchet" path="packages/apps/Trebuchet" remote="github" revision="lineage-18.1" />

    <remove-project name="LineageOS/android_frameworks_base" />
    <project name="KaKE-567/android_frameworks_base" path="frameworks/base" remote="github" revision="lineage-18.1" />
</manifest>
```

### 3. Sync Sources
```bash
repo sync -c -j$(nproc --all) --force-sync --no-clone-bundle --no-tags
```

### 4. Build Environment Setup & Compilation
```bash
source build/envsetup.sh
breakfast miatoll
mka bacon -j$(nproc --all)
```

The output flashable zip will be located at:
`out/target/product/miatoll/VortexOS-18.1-*-UNOFFICIAL-gapps-miatoll.zip` (or `lineage-18.1-*-miatoll.zip`).
