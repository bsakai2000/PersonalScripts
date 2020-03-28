# Copyright 2020 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=7

inherit autotools

DESCRIPTION="ncurses based sudoku game"
HOMEPAGE="https://jubalh.github.io/nudoku"
SRC_URI="https://github.com/jubalh/${PN}/archive/${PV}.tar.gz -> ${P}.tar.gz"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

DEPEND="sys-libs/ncurses:0="
RDEPEND="${DEPEND}"

src_prepare() {
	default
	eautoreconf
}

PATCHES=(
	"${FILESDIR}"/${P}-tinfo.patch
)

src_install() {
	emake DESTDIR="${D}" install
}
