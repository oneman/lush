# Maintainer: David Richards <david@kradradio.com>
pkgname=krad_radio
pkgver=$(git describe --long --dirty | sed 's/\([^-]*-g\)/r\1/;s/-/./g')
pkgrel=$(git log --oneline `git describe`..|wc -l)
pkgdesc="Krad Radio Audio Video System"
url="http://kradradio.com"
arch=('x86_64')
license=('none')
depends=()

package() {
  cd ..
  make DESTDIR="$pkgdir" install
}
