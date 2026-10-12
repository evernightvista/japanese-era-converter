# Japanese Era Converter — KRunner Plugin

KDE Plasma 6 KRunner 插件：在公历年份（1910–2099）与日本年号（元号）之间双向转换。

## 功能

### 正向：公历 → 年号

- 输入 `2027` → 显示 `令和9年 (Reiwa 9)`
- 改元重叠年自动显示两个年号，例如：
  - `1912` → `明治45年 (Meiji 45)` + `大正元年 (Taisho 1)`
  - `1926` → `大正15年 (Taisho 15)` + `昭和元年 (Showa 1)`
  - `1989` → `昭和64年 (Showa 64)` + `平成元年 (Heisei 1)`
  - `2019` → `平成31年 (Heisei 31)` + `令和元年 (Reiwa 1)`

### 反向：年号 → 公历

- 输入年号年数，自动转换为对应的公历年份。例如：
  - `令和8年` / `令和8` / `Reiwa 8` → `2026年 (令和8年 / Reiwa 8)`
  - `令和元年` / `令和1年` / `Reiwa 1` → `2019年 (令和元年 / Reiwa 1)`
  - `昭和64年` → `1989年 (昭和64年 / Showa 64)`
  - `平成31年` → `2019年 (平成31年 / Heisei 31)`
- 支持五个年号：明治 / 大正 / 昭和 / 平成 / 令和
- 支持的输入形式（以令和8年为例）：
  - 汉字：`令和8年`、`令和8`、`令和元年`、`令和1年`
  - 罗马字（大小写不敏感）：`Reiwa 8`、`Reiwa8`、`reiwa8`
  - 拼音（大小写不敏感）：`linghe8年`、`linghe8`、`LingHe8`
- 各年号对应拼音：明治 `mingzhi`、大正 `dazheng`、昭和 `zhaohe`、平成 `pingcheng`、令和 `linghe`
- 拼音示例：
  - `zhaohe8年` → `1933年 (昭和8年 / Showa 8)`
  - `pingcheng31年` → `2019年 (平成31年 / Heisei 31)`
- 年数超过该年号实际跨度时不显示（如 `昭和65年`、`平成32年`）
- 点击结果将**公历年份**复制到剪贴板（如 `2026`）

### 通用

- 输入非有效内容（非纯数字、非可识别的年号年数，或超出 1910–2099 范围）时，KRunner 中不显示本插件
- 正向点击结果复制年号显示文本；反向点击结果复制公历年份

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
