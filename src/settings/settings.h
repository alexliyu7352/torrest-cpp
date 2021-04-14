#ifndef TORREST_SETTINGS_H
#define TORREST_SETTINGS_H

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

namespace torrest {

    enum UserAgent {
        torrest_default,
        libtorrent,
        libtorrent_rasterbar_1_1_0,
        bittorrent_7_5_0,
        bittorrent_7_4_3,
        utorrent_3_4_9,
        utorrent_3_2_0,
        utorrent_2_2_1,
        transmission_2_92,
        deluge_1_3_6_0,
        deluge_1_3_12_0,
        vuze_5_7_3_0,
        ua_num_values
    };

    enum EncryptionPolicy {
        enabled,
        disabled,
        forced,
        ep_num_values
    };

    enum ProxyType {
        none,
        socks4,
        socks5,
        socks5_password,
        socks_http,
        socks_http_password,
        i2psam,
        pt_num_values
    };

    struct Settings {
        int listen_port = 6889;
        std::string listen_interfaces;
        std::string outgoing_interfaces;
        bool disable_dht = false;
        bool disable_upnp = false;
        bool disable_natpmp = false;
        bool disable_lsd = false;
        std::string download_path = "downloads";
        std::string torrents_path = "downloads/torrents";
        UserAgent user_agent = torrest_default;
        int session_save = 30;
        bool tuned_storage = false;
        bool check_available_space = true;
        int connections_limit = 0;
        bool limit_after_buffering = false;
        int max_download_rate = 0;
        int max_upload_rate = 0;
        int share_ratio_limit = 0;
        int seed_time_ratio_limit = 0;
        int seed_time_limit = 0;
        int active_downloads_limit = 3;
        int active_seeds_limit = 5;
        int active_checking_limit = 1;
        int active_dht_limit = 88;
        int active_tracker_limit = 1600;
        int active_lsd_limit = 60;
        int active_limit = 500;
        EncryptionPolicy encryption_policy = enabled;
        ProxyType proxy_type = none;
        int proxy_port = 0;
        std::string proxy_hostname;
        std::string proxy_username;
        std::string proxy_password;
        int64_t buffer_size = 20 * 1024 * 1024;
        int piece_wait_timeout = 60;
        spdlog::level::level_enum service_log_level = spdlog::level::info;
        spdlog::level::level_enum alert_log_level = spdlog::level::critical;
        spdlog::level::level_enum api_log_level = spdlog::level::err;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings,
                                       listen_port,
                                       listen_interfaces,
                                       outgoing_interfaces,
                                       disable_dht,
                                       disable_upnp,
                                       disable_natpmp,
                                       disable_lsd,
                                       download_path,
                                       torrents_path,
                                       user_agent,
                                       session_save,
                                       tuned_storage,
                                       check_available_space,
                                       connections_limit,
                                       limit_after_buffering,
                                       max_download_rate,
                                       max_upload_rate,
                                       share_ratio_limit,
                                       seed_time_ratio_limit,
                                       seed_time_limit,
                                       active_downloads_limit,
                                       active_seeds_limit,
                                       active_checking_limit,
                                       active_dht_limit,
                                       active_tracker_limit,
                                       active_lsd_limit,
                                       active_limit,
                                       encryption_policy,
                                       proxy_type,
                                       proxy_port,
                                       proxy_hostname,
                                       proxy_username,
                                       proxy_password,
                                       buffer_size,
                                       piece_wait_timeout,
                                       service_log_level,
                                       alert_log_level,
                                       api_log_level)

        static Settings load(const std::string &path);

        static Settings parse(const std::string &json);

        void save(const std::string &path);

        std::string dump();

        void validate() const;
    };

    std::string get_user_agent(UserAgent pUserAgent);

}

#endif //TORREST_SETTINGS_H
