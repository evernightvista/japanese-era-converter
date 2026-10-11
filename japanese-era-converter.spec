Name:           japanese-era-converter
Version:        45.0.0
Release:        1%{?dist}
Summary:        KRunner plugin to convert Gregorian years to Japanese era names

License:        MIT
URL:            https://github.com/example/japanese-era-converter
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++
BuildRequires:  gettext
BuildRequires:  extra-cmake-modules >= 6.30.0
BuildRequires:  qt6-qtbase-devel >= 6.5.0
BuildRequires:  kf6-krunner-devel >= 6.30.0
BuildRequires:  kf6-ki18n-devel >= 6.30.0
BuildRequires:  kf6-kcoreaddons-devel >= 6.30.0

Requires:       kf6-krunner
Requires:       kf6-ki18n

%description
A KRunner (KDE Plasma 6) plugin that converts Gregorian calendar years
(1910–2099) to Japanese era names. Based on official Japanese
era transition dates: Meiji, Taisho, Showa, Heisei, and Reiwa.

When the input is not a valid year within 1910–2099, the plugin does
not appear in KRunner results.

%prep
%autosetup -p1

%build
%cmake_kf6
%cmake_build

%install
%cmake_install
%find_lang japanese-era-converter

%files -f japanese-era-converter.lang
%license LICENSE
%doc README.md
%{_qt6_plugindir}/kf6/krunner/japaneseera.so

%changelog
* Sun Oct 11 2026 KairikiFedora <13278297951@sina.cn> - 45.0.0-1
- Initial release
- Support years 1910-2099
- Cover Meiji, Taisho, Showa, Heisei, Reiwa eras
- Copy result to clipboard on selection
