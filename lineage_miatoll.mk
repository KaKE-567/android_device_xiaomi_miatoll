# Copyright (C) 2020 Fluid
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

$(call inherit-product, device/xiaomi/miatoll/device.mk)
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Bootanimation Resolution
TARGET_BOOT_ANIMATION_RES := 1080

TARGET_UNOFFICIAL_BUILD_ID := gapps

PRODUCT_NAME := lineage_miatoll
PRODUCT_DEVICE := miatoll
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := SM6250
PRODUCT_MANUFACTURER := Xiaomi

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi
TARGET_SUPPORTS_GOOGLE_RECORDER := true

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=curtana \
    TARGET_DEVICE=curtana \
    PRODUCT_DEVICE=curtana \
    PRODUCT_MODEL="Redmi Note 9 Pro" \
    PRODUCT_BRAND=Redmi \
    BUILD_NUMBER="V12.5.5.0.RJWINXM" \
    PRIVATE_BUILD_DESC="curtana-user 11 RKQ1.200826.002 V12.5.5.0.RJWINXM release-keys"

BUILD_FINGERPRINT := Redmi/curtana/curtana:11/RKQ1.200826.002/V12.5.5.0.RJWINXM:user/release-keys
BUILD_DISPLAY_ID := RKQ1.200826.002
TARGET_BUILD_FLAVOR := curtana-user

# Release keys signing
PRODUCT_DEFAULT_DEV_CERTIFICATE := build/make/target/product/security/releasekey
PRODUCT_OTA_PUBLIC_KEYS := build/make/target/product/security/releasekey
BUILD_KEYS := release-keys


# Remove unwanted apps and residue
PRODUCT_PACKAGES := $(filter-out \
    AudioFX \
    ExactCalculator \
    Calculator \
    Etar \
    Calendar \
    CalendarProvider \
    Contacts \
    LocalContactsBackup \
    default-permissions_org.calyxos.backup.contacts \
    FM2 \
    libqcomfm_jni \
    libfmjni \
    fm_helium \
    libfm-hci \
    qcom.fmradio \
    qcom.fmradio.xml \
    messaging \
    com.android.messaging_whitelist.xml \
    Eleven \
    Music \
    MusicFX \
    privapp_whitelist_org.lineageos.eleven \
    Recorder \
    Snap \
    Camera2 \
    LatinIME \
    GamingMode \
    privapp_whitelist_org.exthmui.game.xml,$(PRODUCT_PACKAGES))
