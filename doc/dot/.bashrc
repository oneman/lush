#
# ~/.bashrc
#

# If not running interactively, don't do anything

alias s='sensors coretemp-isa-0'
alias ls='ls -Q --color=auto'
alias htop='nice htop'
alias weston='weston --width=1920 --height=1080'
alias bemenu='/usr/local/bin/bemenu-run --fn "M+ 2mn Bold 55"'
alias pacman='sudo pacman'
alias nload='nload -u H'
alias like='xmms2 info >> ~/music/liked'
alias lc='ls -I*.o -I*.lo -I*.la'
alias kc='cd ~/kode/krad_radio'
alias marson='wol c8:60:00:c6:d8:ff'
alias ydl='youtube-dl -f bestvideo+bestaudio'
alias wm="/home/oneman/kode/loliwm_old/target/src/loliwm"

alias bashallbash='bash *.bash'
alias bashbash='bashallbash'
alias bake='bashbash'
alias n26='grep -n26 '
export bashallbash bashbash bake n26

alias rg='grep -r'
alias rgrep='grep -r'
alias cgrep="grep -r --include=*.h --include=*.c"

alias gs='git status'
alias ga='git add'
alias gd='git diff'
alias gfa='git fetch --all'

alias xp='xmms2 prev'
alias xn='xmms2 next'
alias xl='xmms2 list'
alias xs='xmms2 status'
alias xi='xmms2 info'
alias xc='xmms2 current'

alias radio2="KRAD_WEB_PORT=4000 krad_radio radio2"
alias r2="radio2"
alias radio3="KRAD_WEB_PORT=3000 krad_radio radio3"
alias r3="radio3"

GDK_BACKEND=x11
#GDK_BACKEND=wayland
BEMENU_BACKEND=wayland
TERMINAL=terminology
export BEMENU_BACKEND GDK_BACKEND TERMINAL

WLC_DIM=0.7
WLC_BG=0
export WLC_BG WLC_DIM

KRAD_WEB_PORT=5000
KRAD_WEB_ROOT=$HOME/kode/krad_radio/web/rack
KRAD_XFER_INCOMING=$HOME/uploads
export KRAD_WEB_PORT KRAD_WEB_ROOT KRAD_XFER_INCOMING
[[ -f ~/.kradrc ]] && . ~/.kradrc

# roflcoper zone
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
LD_LIBRARY_PATH=/usr/local/lib:/home/oneman/kode/loliwm_old/target/src
export LD_LIBRARY_PATH PKG_CONFIG_PATH

PS1='\[\033[34m\][\[\033[37m\]\w\[\033[34m\]]\[\033[0m\]% '
PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/opt/cuda/bin:/opt/cuda/nsight_compute:/opt/cuda/nsight_systems/bin:/home/demo/.dotnet/tools:/var/lib/flatpak/exports/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl:/var/lib/snapd/snap/bin:/home/demo/.cargo/bin:/home/demo/.cargo/bin
