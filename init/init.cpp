/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#include <cstring>
#include <sys/sysinfo.h>
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "init_common.h"
#include "vendor_init.h"

using android::base::GetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "bootimage.",
    "odm.",
    "product.",
    "system.",
    "system_ext.",
    "vendor.",
};

void set_ro_build_prop(const std::string &source, const std::string &prop,
        const std::string &value, bool product = false) {
    std::string prop_name;

    if (product) {
        prop_name = "ro.product." + source + prop;
    } else {
        prop_name = "ro." + source + "build." + prop;
    }

    property_override(prop_name.c_str(), value.c_str(), false);
}

void set_device_props(const std::string fingerprint, const std::string description,
        const std::string in_brand, const std::string in_device, const std::string model) {
    std::string brand = in_brand;
    std::string product = in_device;
    std::string device = in_device;
    std::string release = "11";
    std::string id = "RKQ1.200826.002";
    std::string incremental = "";
    std::string type = "user";
    std::string tags = "release-keys";

    // Parse fingerprint: brand/product/device:release/id/incremental:type/tags
    size_t colon1 = fingerprint.find(':');
    size_t colon2 = (colon1 != std::string::npos) ? fingerprint.find(':', colon1 + 1) : std::string::npos;
    if (colon1 != std::string::npos && colon2 != std::string::npos) {
        std::string part1 = fingerprint.substr(0, colon1); // brand/product/device
        std::string part2 = fingerprint.substr(colon1 + 1, colon2 - colon1 - 1); // release/id/incremental
        std::string part3 = fingerprint.substr(colon2 + 1); // type/tags

        // parse part1
        size_t s1 = part1.find('/');
        size_t s2 = (s1 != std::string::npos) ? part1.find('/', s1 + 1) : std::string::npos;
        if (s1 != std::string::npos && s2 != std::string::npos) {
            brand = part1.substr(0, s1);
            product = part1.substr(s1 + 1, s2 - s1 - 1);
            device = part1.substr(s2 + 1);
        }

        // parse part2
        size_t s3 = part2.find('/');
        size_t s4 = (s3 != std::string::npos) ? part2.find('/', s3 + 1) : std::string::npos;
        if (s3 != std::string::npos && s4 != std::string::npos) {
            release = part2.substr(0, s3);
            id = part2.substr(s3 + 1, s4 - s3 - 1);
            incremental = part2.substr(s4 + 1);
        }

        // parse part3
        size_t s5 = part3.find('/');
        if (s5 != std::string::npos) {
            type = part3.substr(0, s5);
            tags = part3.substr(s5 + 1);
        }
    }

    std::string date_utc = GetProperty("ro.build.date.utc", "");
    std::string date = GetProperty("ro.build.date", "");
    for (const auto &source : ro_props_default_source_order) {
        set_ro_build_prop(source, "fingerprint", fingerprint);
        set_ro_build_prop(source, "brand", brand, true);
        set_ro_build_prop(source, "device", device, true);
        set_ro_build_prop(source, "name", product, true);
        set_ro_build_prop(source, "model", model, true);
        set_ro_build_prop(source, "id", id);
        set_ro_build_prop(source, "version.release", release);
        if (!incremental.empty()) {
            set_ro_build_prop(source, "version.incremental", incremental);
        }
        set_ro_build_prop(source, "type", type);
        set_ro_build_prop(source, "tags", tags);
        if (!date_utc.empty()) {
            set_ro_build_prop(source, "date.utc", date_utc);
        }
        if (!date.empty()) {
            set_ro_build_prop(source, "date", date);
        }
        set_ro_build_prop(source, "user", "android-build");
        set_ro_build_prop(source, "host", "buildhost");
    }

    property_override("ro.build.fingerprint", fingerprint.c_str());
    property_override("ro.build.description", description.c_str());
    property_override("ro.build.id", id.c_str());
    property_override("ro.build.display.id", id.c_str());
    if (!incremental.empty()) {
        property_override("ro.build.version.incremental", incremental.c_str());
    }
    property_override("ro.build.version.release", release.c_str());
    property_override("ro.build.type", type.c_str());
    property_override("ro.build.tags", tags.c_str());
    property_override("ro.build.flavor", (device + "-user").c_str());
    property_override("ro.debuggable", "0");
    property_override("ro.secure", "1");
    property_override("ro.adb.secure", "1");
    property_override("ro.build.user", "android-build");
    property_override("ro.build.host", "buildhost");
    property_override("ro.bootloader", device.c_str());
    property_override("ro.boot.bootloader", device.c_str());

    property_override("ro.product.brand", brand.c_str());
    property_override("ro.product.device", device.c_str());
    property_override("ro.product.name", product.c_str());
    property_override("ro.product.model", model.c_str());
}

void load_device_properties() {
    std::string hwname = GetProperty("ro.boot.hwname", "");
    std::string region = GetProperty("ro.boot.hwc", "");

    if (hwname == "curtana") {
        if (region == "Global_TWO" || region == "Global_PA" || region == "Japan") {
            set_device_props(
                    "Redmi/curtana_global/curtana:11/RKQ1.200826.002/V12.5.5.0.RJWMIXM:user/release-keys",
                    "curtana_global-user 11 RKQ1.200826.002 V12.5.5.0.RJWMIXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9S");
        } else if (region == "India") {
            set_device_props(
                    "Redmi/curtana/curtana:11/RKQ1.200826.002/V12.5.5.0.RJWINXM:user/release-keys",
                    "curtana-user 11 RKQ1.200826.002 V12.5.5.0.RJWINXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9 Pro");
        } else {
            set_device_props(
                    "Redmi/curtana_global/curtana:11/RKQ1.200826.002/V12.5.5.0.RJWMIXM:user/release-keys",
                    "curtana_global-user 11 RKQ1.200826.002 V12.5.5.0.RJWMIXM release-keys",
                    "Redmi", "curtana", "Redmi Note 9S");
        }
    } else if (hwname == "excalibur") {
        set_device_props(
                "Redmi/excalibur_in/excalibur:11/RKQ1.200826.002/V12.5.6.0.RJXINXM:user/release-keys",
                "excalibur_in-user 11 RKQ1.200826.002 V12.5.6.0.RJXINXM release-keys",
                "Redmi", "excalibur", "Redmi Note 9 Pro Max");
    } else if (hwname == "gram") {
        set_device_props(
                "POCO/gram_in/gram:11/RKQ1.200826.002/V12.5.6.0.RJPINXM:user/release-keys",
                "gram_in-user 11 RKQ1.200826.002 V12.5.6.0.RJPINXM release-keys",
                "POCO", "gram", "POCO M2 Pro");
    } else if (hwname == "joyeuse") {
        set_device_props(
                "Redmi/joyeuse_global/joyeuse:11/RKQ1.200826.002/V12.5.7.0.RJZMIXM:user/release-keys",
                "joyeuse_global-user 11 RKQ1.200826.002 V12.5.7.0.RJZMIXM release-keys",
                "Redmi", "joyeuse", "Redmi Note 9 Pro");
    } else {
        set_device_props(
                "Redmi/curtana_global/curtana:11/RKQ1.200826.002/V12.5.5.0.RJWMIXM:user/release-keys",
                "curtana_global-user 11 RKQ1.200826.002 V12.5.5.0.RJWMIXM release-keys",
                "Redmi", "curtana", "Redmi Note 9S");
    }
}

void vendor_load_properties() {
    load_common_properties();
    load_device_properties();
}