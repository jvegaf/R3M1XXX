# 7Semi ADS7830 - Fixed Version

## About This Library

This is a **locally patched version** of the [7Semi ADS7830 Arduino Library](https://github.com/7semi-solutions/7Semi-ADS7830-Arduino-Library) (v1.0.0).

The original library has a C++ compilation error when compiled with `-fpermissive` flag disabled (which is the default in modern compilers).

---

## What Was Fixed?

### Original Error
```
error: extra qualification 'ADS7830_7semi::' on member 'SINGLE_ENDED_CH' [-fpermissive]
```

### Root Cause
In the original `7semi_ADS7830.h` (line 53), the static const array `SINGLE_ENDED_CH` was **defined** (initialized) inside the class declaration with the qualification `ADS7830_7semi::`, which is not valid C++ syntax.

**Incorrect code (original):**
```cpp
// In 7semi_ADS7830.h
private:
  const uint8_t ADS7830_7semi::SINGLE_ENDED_CH[8] = {  // ❌ ERROR
    0x80, 0xC0, 0x90, 0xD0, 0xA0, 0xE0, 0xB0, 0xF0
  };
```

### Solution Applied

Following standard C++ practices, the array is now **declared** in the `.h` file and **defined** in the `.cpp` file:

**Fixed code:**

`7semi_ADS7830.h`:
```cpp
private:
  // AIDEV-NOTE: Static array declaration (definition moved to .cpp to fix compilation error)
  static const uint8_t SINGLE_ENDED_CH[8];  // ✅ Declaration only
```

`7semi_ADS7830.cpp`:
```cpp
// AIDEV-NOTE: Definition of static const array moved from .h to .cpp
const uint8_t ADS7830_7semi::SINGLE_ENDED_CH[8] = {
  0x80,  // CH0
  0xC0,  // CH1
  0x90,  // CH2
  0xD0,  // CH3
  0xA0,  // CH4
  0xE0,  // CH5
  0xB0,  // CH6
  0xF0   // CH7
};
```

---

## Why Use This Local Version?

1. ✅ **Permanent fix**: Survives `pio run --target clean` and library updates
2. ✅ **Versionable**: Included in Git repository for team consistency
3. ✅ **Standards compliant**: Follows C++17 best practices
4. ✅ **Documented**: Clearly explains why this fork exists

---

## Files Modified

- `src/7semi_ADS7830.h`: Changed array definition to declaration
- `src/7semi_ADS7830.cpp`: Added array definition after include
- `library.properties`: Updated name and version to indicate fix

---

## Original Library Info

- **Name**: 7Semi ADS7830
- **Version**: 1.0.0 (base)
- **Author**: 7Semi <info@7semi.com>
- **URL**: https://github.com/7semi-solutions/7Semi-ADS7830-Arduino-Library
- **License**: MIT

---

## Reporting to Upstream

This fix should ideally be reported to the original repository as an issue or pull request so the maintainers can incorporate it into the official release.

**Repository**: https://github.com/7semi-solutions/7Semi-ADS7830-Arduino-Library

---

## Maintenance Notes

- If the upstream library releases a new version with bug fixes or features, this local version will need to be manually updated
- Consider periodically checking the upstream repository for updates
- All changes to this library should be documented with `AIDEV-NOTE` comments

---

**Fixed by**: R3M1XXX  
**Date**: December 14, 2024  
**Base version**: 1.0.0  
**Fixed version**: 1.0.0-fixed
