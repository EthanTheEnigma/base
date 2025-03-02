#! /bin/bash

SEMUPDATE_PWD=`pwd`
SEMUPDATE_BUILD="${HOME}/deploy"
SEMUPDATE_DEPOT="${HOME}/depot"
SEMUPDATE_DIR="${SEMUPDATE_BUILD}/${BRANCH_NAME}"
SEMUPDATE_APT='DEBIAN_FRONTEND=noninteractive apt-get'
SEMUPDATE_DEST="https://${GITHUB_TOKEN}:x-oauth-basic@github.com/redeclipse/deploy.git"
SEMUPDATE_APPIMAGE="https://github.com/redeclipse/appimage-builder.git"
SEMUPDATE_APPIMAGE_GH_DEST="redeclipse/deploy"
SEMUPDATE_MODULES=`cat "${SEMUPDATE_PWD}/.gitmodules" | grep '\[submodule "[^.]' | sed -e 's/^.submodule..//;s/..$//' | tr "\n" " " | sed -e 's/ $//'`
SEMUPDATE_ALLMODS="base ${SEMUPDATE_MODULES}"

SEMUPDATE_VERSION_MAJOR=`sed -n 's/.define VERSION_MAJOR \([0-9]*\)/\1/p' src/engine/version.h`
SEMUPDATE_VERSION_MINOR=`sed -n 's/.define VERSION_MINOR \([0-9]*\)/\1/p' src/engine/version.h`
SEMUPDATE_VERSION_PATCH=`sed -n 's/.define VERSION_PATCH \([0-9]*\)/\1/p' src/engine/version.h`
SEMUPDATE_VERSION="${SEMUPDATE_VERSION_MAJOR}.${SEMUPDATE_VERSION_MINOR}.${SEMUPDATE_VERSION_PATCH}"
SEMUPDATE_STEAM_APPID=`sed -n 's/.define VERSION_STEAM_APPID \([0-9]*\)/\1/p' src/engine/version.h`
SEMUPDATE_STEAM_DEPOT=`sed -n 's/.define VERSION_STEAM_DEPOT \([0-9]*\)/\1/p' src/engine/version.h`
SEMUPDATE_DESCRIPTION="${BRANCH_NAME}:${SEMAPHORE_BUILD_NUMBER} from ${REVISION} for v${SEMUPDATE_VERSION}"
SEMUPDATE_BRANCH="${BRANCH_NAME}"
if [ "${SEMUPDATE_BRANCH}" = "master" ]; then SEMUPDATE_BRANCH="devel"; fi

semupdate_setup() {
    echo "########## SETTING UP ${BRANCH_NAME} ##########"
    git config --global user.email "noreply@redeclipse.net" || return 1
    git config --global user.name "Red Eclipse" || return 1
    git config --global credential.helper store || return 1
    echo "https://${GITHUB_TOKEN}:x-oauth-basic@github.com" > "${HOME}/.git-credentials"
    rm -rf "${SEMUPDATE_BUILD}" || return 1
    rm -rf "${SEMUPDATE_PWD}/data" || return 1
    pushd "${HOME}" || return 1
    git clone --depth 1 "${SEMUPDATE_DEST}" || return 1
    popd || return 1
    mkdir -pv "${SEMUPDATE_DIR}" || return 1
    for i in ${SEMUPDATE_ALLMODS}; do
        if [ "${i}" != "base" ]; then
            git submodule update --init --depth 5 "data/${i}" || return 1
        fi
    done
    echo "--------------------------------------------------------------------------------"
    return 0
}

semupdate_wait() {
    pushd "${SEMUPDATE_BUILD}" || return 1
    SEMUPDATE_CURPRC=1
    echo "Waiting for macOS build to complete.." # Will wait up to 15 minutes before failing
    for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
        SEMUPDATE_CURBIN=`cat "${SEMUPDATE_DIR}/bins.txt"`
        SEMUPDATE_CURMAC=`cat "${SEMUPDATE_DIR}/macos.txt"`
        echo ""
        echo "Binaries: ${SEMUPDATE_CURBIN} macOS: ${SEMUPDATE_CURMAC}"
        if [ "${SEMUPDATE_CURBIN}" != "${SEMUPDATE_CURMAC}" ]; then
            echo "[${i}] Sleep for 30 seconds.."
            sleep 30s || return 1
            git pull || return 1
        else
            SEMUPDATE_CURPRC=0
            break
        fi
    done
    popd || return 1
    return ${SEMUPDATE_CURPRC}
}

semupdate_appimage() {
    echo "########## BUILDING APPIMAGE ##########"
    sudo ${SEMUPDATE_APT} update || return 1
    sudo ${SEMUPDATE_APT} -fy install build-essential multiarch-support gcc-multilib g++-multilib zlib1g-dev libsdl2-dev libsndfile1-dev libalut-dev libopenal-dev libsdl2-image-dev jq zsync || return 1
    sudo ${SEMUPDATE_APT} clean || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## CLONING REPOSITORY ##########"
    pushd "${HOME}" || return 1
    git clone --depth 1 "${SEMUPDATE_APPIMAGE}" appimage || return 1
    pushd appimage || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## BUILDING APPIMAGE ##########"
    export BRANCH="${BRANCH_NAME}"
    export ARCH=x86_64
    export COMMIT=${REVISION}
    export BUILD_SERVER=1
    export BUILD_CLIENT=1
    export PLATFORM_BUILD=${SEMAPHORE_BUILD_NUMBER}
    export PLATFORM_BRANCH="${BRANCH_NAME}"
    export PLATFORM_REVISION="${REVISION}"
    export NO_UPDATE=true
    export BUILD="${SEMUPDATE_PWD}"
    bash build-appimages.sh || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## DEPLOYING GITHUB RELEASE ##########"
    export GITHUB_TOKEN="${GITHUB_TOKEN}"
    export REPO_SLUG="${SEMUPDATE_APPIMAGE_GH_DEST}"
    export COMMIT=$(git rev-parse ${REVISION})
    bash github-release.sh || return 1
    echo "--------------------------------------------------------------------------------"

    popd || return 1
    # Clear the appimage building directory to save space. (not needed with separated deploy servers)
    # rm -rf appimage
    popd || return 1
    return 0
}

semupdate_steamdbg() {
    echo "########## HOME DIRECTORY LISTING ##########"
    date || return 1
    find "${HOME}" -maxdepth 1 -printf "%c | %M | %u:%g | %Y | %p | %l\n" || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## CACHE DIRECTORY LISTING ##########"
    date || return 1
    find "${SEMAPHORE_CACHE_DIR}" -printf "%c | %M | %u:%g | %Y | %p | %l\n" || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## FREE DISK SPACE CHECK ##########"
    df -h || return 1
    echo "--------------------------------------------------------------------------------"

    if [ $1 != 0 ]; then
        echo "########## PRINTING LOGS ##########"
        cat "${HOME}/Steam/logs/stderr.txt"
        echo "--------------------------------------------------------------------------------"
    fi
    return 0
}

semupdate_steam() {
    echo "########## BUILDING STEAM DEPOT ##########"

    sudo ${SEMUPDATE_APT} update || exit 1
    sudo ${SEMUPDATE_APT} -fy install libc6-i386 || exit 1
    #sudo ${SEMUPDATE_APT} clean || exit 1

    mkdir -pv "${SEMUPDATE_DEPOT}" || return 1
    pushd "${SEMUPDATE_PWD}/src/install/steam" || return 1

    for i in *; do
        if [ ! -d "${i}" ] && [ -e "${i}" ]; then
            sed -e "s/~REPAPPID~/${SEMUPDATE_STEAM_APPID}/g;s/~REPDESC~/${SEMUPDATE_DESCRIPTION}/g;s/~REPBRANCH~/${SEMUPDATE_BRANCH}/g;s/~REPDEPOT~/${SEMUPDATE_STEAM_DEPOT}/g" "${i}" > "${SEMUPDATE_DEPOT}/${i}" || return 1
        fi
    done
    popd || return 1
    echo "--------------------------------------------------------------------------------"

    echo "########## SYMLINKING CACHE ##########"
    mkdir -pv "${SEMAPHORE_CACHE_DIR}/Steam-dot" || return 1
    ln -sv "${SEMAPHORE_CACHE_DIR}/Steam-dot" "${HOME}/.steam" || return 1
    mkdir -pv "${SEMAPHORE_CACHE_DIR}/Steam" || return 1
    ln -sv "${SEMAPHORE_CACHE_DIR}/Steam" "${HOME}/Steam" || return 1
    for i in output package public; do
        mkdir -pv "${SEMAPHORE_CACHE_DIR}/Steam-${i}" || return 1
        ln -sv "${SEMAPHORE_CACHE_DIR}/Steam-${i}" "${SEMUPDATE_DEPOT}/${i}" || return 1
    done
    echo "--------------------------------------------------------------------------------"

    echo "########## ARCHIVING REPOSITORIES ##########"
    for i in ${SEMUPDATE_ALLMODS}; do
        if [ "${i}" = "base" ]; then
            SEMUPDATE_MODDIR="${SEMUPDATE_DEPOT}/content"
            SEMUPDATE_GITDIR="${SEMUPDATE_PWD}"
            SEMUPDATE_ARCHBR="${BRANCH_NAME}"
        else
            SEMUPDATE_MODDIR="${SEMUPDATE_DEPOT}/content/data/${i}"
            SEMUPDATE_GITDIR="${SEMUPDATE_PWD}/data/${i}"
            git submodule update --init --depth 5 "data/${i}" || return 1
            pushd "${SEMUPDATE_GITDIR}" || return 1
            SEMUPDATE_ARCHBR=`git rev-parse HEAD`
            popd || return 1
        fi
        mkdir -pv "${SEMUPDATE_MODDIR}" || return 1
        pushd "${SEMUPDATE_GITDIR}" || return 1
        (git archive ${SEMUPDATE_ARCHBR} | tar -x -C "${SEMUPDATE_MODDIR}") || return 1
        if [ "${i}" = "base" ]; then
            # Steam build on Windows HATES SYMLINKS
            rm -rfv "${SEMUPDATE_MODDIR}/bin/redeclipse.app" "${SEMUPDATE_MODDIR}/readme.md" "${SEMUPDATE_MODDIR}/doc/commands.txt" || return 1
            cp -RLfv "${SEMUPDATE_GITDIR}/config/usage.cfg" "${SEMUPDATE_MODDIR}/doc/commands.txt" || return 1
            cp -RLfv "${SEMUPDATE_GITDIR}/bin/redeclipse.app" "${SEMUPDATE_MODDIR}/bin/redeclipse.app" || return 1
        fi
        popd || return 1
    done
    echo "--------------------------------------------------------------------------------"

    echo "########## GRABBING DEPLOYMENT BINARIES ##########"
    echo "steam" > "${SEMUPDATE_DEPOT}/content/branch.txt" || return 1
    unzip -o "${SEMUPDATE_DIR}/windows.zip" -d "${SEMUPDATE_DEPOT}/content" || return 1
    tar --gzip --extract --verbose --overwrite --file="${SEMUPDATE_DIR}/linux.tar.gz" --directory="${SEMUPDATE_DEPOT}/content"
    tar --gzip --extract --verbose --overwrite --file="${SEMUPDATE_DIR}/macos.tar.gz" --directory="${SEMUPDATE_DEPOT}/content"
    echo "--------------------------------------------------------------------------------"

    echo "########## SETTING UP STEAMCMD ##########"
    pushd "${SEMUPDATE_DEPOT}" || return 1
    curl -sqL "https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz" | tar zxvf -
    chmod --verbose +x "linux32/steamcmd" || return 1
    export LD_LIBRARY_PATH="${SEMUPDATE_DEPOT}/linux32:${LD_LIBRARY_PATH}"
    STEAM_ARGS="+login redeclipsenet ${STEAM_TOKEN} +run_app_build_http app_build.vdf +quit"
    if [ "${STEAM_GUARD}" != "0" ]; then STEAM_ARGS="+set_steam_guard_code ${STEAM_GUARD} ${STEAM_ARGS}"; fi
    echo "--------------------------------------------------------------------------------"

    semupdate_steamdbg 0 || return 1

    echo "########## RUNNING STEAMCMD ##########"
    STEAM_EXECS=0
    ./linux32/steamcmd ${STEAM_ARGS}
    while [ $? -eq 42 ] && [ ${STEAM_EXECS} -lt 2 ]; do
        echo "--------------------------------------------------------------------------------"
        semupdate_steamdbg 1 || return 1

        STEAM_EXECS=$(( STEAM_EXECS + 1 ))
        echo "########## RUNNING STEAMCMD [RETRY: ${STEAM_EXECS}] ##########"
        ./linux32/steamcmd ${STEAM_ARGS}
    done
    echo "--------------------------------------------------------------------------------"

    semupdate_steamdbg 1 || return 1

    popd || return 1
    return 0
}

if [ "${BRANCH_NAME}" = master ]; then
    semupdate_setup || exit 1
    case $1 in
        appimage)
            semupdate_appimage || exit 1
            ;;
        steam)
            semupdate_steam || exit 1
            ;;
        *)
            echo "Usage: $0 <appimage|steam>"
            ;;
    esac
fi
echo "done."
