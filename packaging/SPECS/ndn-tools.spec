Name:       ndn-tools
Version:    0.6.1
Release:    1%{?dist}
Summary:    NDN Essential Tools
License:    GPLv3+
URL:        https://github.com/named-data/ndn-tools.git
BuildRequires:  pkgconfig libpcap-devel libndn-cxx-devel
BuildRequires:  python2

%description
ndn-tools is a collection of essential tools for Named Data Networking.
These tools are recommended to be installed on all NDN nodes.

%global debug_package %{nil}

%prep
rm -rf %{name}
git clone %{url}
cd %{name}
git fetch --all --tags --prune
git checkout tags/%{name}-%{version}
find -type f ! \( -name \*.sh -o -name \*.py -o -name \*.pl \) -exec chmod -x {} +
find . -name \*.orig -exec rm {} +

%build
find . -type f -exec chmod u+w {} +
cd %{name}
CXXFLAGS="%{optflags} -std=c++11" \
%{__python2} ./waf --with-tests --prefix=%{_prefix} --bindir=%{_bindir} --libdir=%{_libdir} \
 --datadir=%{_datadir} --sysconfdir=%{_sysconfdir} configure

%{__python2} ./waf -v %{?_smp_mflags}

%install
cd %{name}
chmod a+x waf
./waf install --destdir=%{buildroot} --prefix=%{_prefix} \
 --bindir=%{_bindir} --datadir=%{_datadir}

%check
#build/unit-tests-core
#build/unit-tests-daemon
#build/unit-tests-rib

%files
%{_bindir}/*
%{_datarootdir}/* 
%{_mandir}

%changelog
* Sun Jun 17 2018 Catalin Iordache <catalinniordache at gmail dot com> - 0.6.1
- Initial NDN Tools packaging
