# Japanese Era Converter — KRunner Plugin

KDE Plasma 6 KRunner 插件：输入公历年份（1910–2099），显示对应的日本年号（元号）。

## 功能

- 输入 `2027` → 显示 `令和9年 (Reiwa 9)`
- 改元重叠年自动显示两个年号，例如：
  - `1912` → `明治45年 (Meiji 45)` + `大正元年 (Taisho 1)`
  - `1926` → `大正15年 (Taisho 15)` + `昭和元年 (Showa 1)`
  - `1989` → `昭和64年 (Showa 64)` + `平成元年 (Heisei 1)`
  - `2019` → `平成31年 (Heisei 31)` + `令和元年 (Reiwa 1)`
- 输入非有效年份（不在 1910–2099 范围内，或非数字）时，KRunner 中不显示本插件
- 点击结果将年号文本复制到剪贴板

## 年号数据（日本官方改元日期）

| 年号 | 罗马字 | 开始 | 结束 |
|------|--------|------|------|
| 明治 | Meiji  | 1868-10-23 | 1912-07-29 |
| 大正 | Taisho | 1912-07-30 | 1926-12-24 |
| 昭和 | Showa  | 1926-12-25 | 1989-01-07 |
| 平成 | Heisei | 1989-01-08 | 2019-04-30 |
| 令和 | Reiwa  | 2019-05-01 | 现在 |

数据来源：日本内阁府/总务省公布的元号一覧。

## 构建依赖

- CMake ≥ 3.16
- Qt6 ≥ 6.5（Core, Gui, Widgets）
- KDE Frameworks 6 ≥ 6.0（Runner, I18n, CoreAddons）
- Extra CMake Modules ≥ 6.0
- C++17 编译器

### Fedora

```bash
sudo dnf install cmake gcc-c++ extra-cmake-modules \
    qt6-qtbase-devel kf6-krunner-devel kf6-ki18n-devel kf6-kcoreaddons-devel
```

### openSUSE

```bash
sudo zypper install cmake gcc-c++ extra-cmake-modules \
    qt6-core-devel kf6-krunner-devel kf6-ki18n-devel kf6-kcoreaddons-devel
```

### Debian / Ubuntu

```bash
sudo apt install cmake build-essential extra-cmake-modules \
    qt6-base-dev libkf6runner-dev libkf6i18n-dev libkf6coreaddons-dev
```

## 编译安装

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
sudo make install
```

安装后重启 KRunner：

```bash
kquitapp6 krunner 2>/dev/null
# 然后按 Alt+F2 或 Alt+Space 重新启动
```

或注销重新登录。

## RPM 打包

```bash
# 生成源码包
tar czf japanese-era-converter-1.0.0.tar.gz japanese-era-converter-1.0.0/

# 构建 RPM（Fedora）
rpmbuild -ta japanese-era-converter-1.0.0.tar.gz
# 或使用 spec 文件
rpmbuild -bb japanese-era-converter.spec
```

## 许可证

MIT License
