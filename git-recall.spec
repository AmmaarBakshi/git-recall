Name:           git-recall
Version:        1.0.0
Release:        1%{?dist}
Summary:        Your personal git standup tool

License:        MIT
URL:            https://github.com/AMMAAR-IC/git-recall
Source0:        %{url}/archive/v%{version}/git-recall-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
Requires:       git

%description
git-recall is a lightweight command-line tool written in pure C.
See what you committed by day, week, month, or year.
Perfect for daily standups and weekly reviews.

%prep
%autosetup

%build
make %{?_smp_mflags}

%install
install -Dm755 git-recall %{buildroot}%{_bindir}/git-recall

%files
%{_bindir}/git-recall
%license LICENSE
%doc README.md

%changelog
* Sat Apr 11 2026 AMMAAR-IC - 1.0.0-1
- Initial release