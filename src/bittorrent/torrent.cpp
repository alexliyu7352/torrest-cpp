#include "torrent.h"

#include <experimental/filesystem>

#include "libtorrent/torrent_status.hpp"
#include "libtorrent/torrent_info.hpp"
#include "service.h"
#include "file.h"

namespace torrest {

    Torrent::Torrent(Service *pService, libtorrent::torrent_handle pHandle, std::string pInfoHash)
            : mService(pService),
              mLogger(pService->mLogger),
              mHandle(std::move(pHandle)),
              mInfoHash(std::move(pInfoHash)),
              mHasMetadata(false),
              mClosed(false) {

        auto flags = mHandle.flags();
        auto status = mHandle.status(libtorrent::torrent_handle::query_name);

        mPaused = (flags & libtorrent::torrent_flags::paused) && !(flags & libtorrent::torrent_flags::auto_managed);
        mDefaultName = status.name.empty() ? mInfoHash : status.name;

        if (status.has_metadata) {
            handle_metadata_received();
        }
    }

    Torrent::~Torrent() {
        mClosed = true;
    }

    void Torrent::handle_metadata_received() {
        mLogger->debug("operation=handle_metadata_received");
        std::lock_guard<std::mutex> lock(mMutex);
        auto torrentFile = mHandle.torrent_file();
        auto files = torrentFile->files();

        mFiles.clear();
        for (int i = 0; i < torrentFile->num_files(); i++) {
            mFiles.emplace_back(std::make_shared<File>(this, files, libtorrent::file_index_t(i)));
        }

        mHasMetadata = true;
    }

    std::string Torrent::get_info_hash() {
        return mInfoHash;
    }

    void Torrent::pause() {
        mLogger->debug("operation=pause, message='Pausing torrent', infoHash={}", mInfoHash);
        std::lock_guard<std::mutex> lock(mMutex);
        mHandle.unset_flags(libtorrent::torrent_flags::auto_managed);
        mHandle.pause(libtorrent::torrent_handle::clear_disk_cache);
        mPaused = true;
    }

    void Torrent::resume() {
        mLogger->debug("operation=resume, message='Resuming torrent', infoHash={}", mInfoHash);
        std::lock_guard<std::mutex> lock(mMutex);
        mHandle.set_flags(libtorrent::torrent_flags::auto_managed);
        mPaused = false;
    }

    void Torrent::check_available_space() {
        if (!mService->mSettings.check_available_space) {
            return;
        }

        auto status = mHandle.status(libtorrent::torrent_handle::query_accurate_download_counters
                                     | libtorrent::torrent_handle::query_save_path
                                     | libtorrent::torrent_handle::query_name);

        if (!status.has_metadata) {
            mLogger->warn("operation=check_available_space, message='No metadata available', infoHash={}", mInfoHash);
            return;
        }

        auto spaceInfo = std::experimental::filesystem::space(mService->mSettings.download_path);
        if (spaceInfo.free < status.total - status.total_done) {
            mLogger->warn("operation=check_available_space, message='Insufficient space on {}', infoHash={}",
                          status.save_path, mInfoHash);
            pause();
        }
    }

}
