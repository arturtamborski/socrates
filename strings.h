#ifndef STRINGS_H
#define STRINGS_H

// main.cpp
// --------

#define STR_ORG_NAME	"arturtamborski"
#define STR_ORG_DOMAIN	"pl"

#define STR_APP_NAME	"Socrates"
#define STR_APP_VERSION	"1.0.0"


// mainwindow.cpp
// --------------

#define STR_READY   " Ready."
#define STR_RUNNING " Running..."

#define STR_LOCAL	"./stream.mp4"
#define STR_REMOTE	"https://videos3.earthcam.com/fecnetwork/" \
                     "AbbeyRoadHD1.flv/chunklist_w89583932.m3u8"

#define STR_ERROR	"Error!"
#define STR_START	"Start"
#define STR_STOP	"Stop"


// server.cpp
// ----------

#define STR_SERVER	"Server: "
#define STR_SERVER_STARTING	STR_SERVER "starting..."

#define STR_SOCKET	"Socket: "
#define STR_SOCKET_TIMEOUT	STR_SOCKET "timeout! skipping..."

#define STR_FRAME_INVALID STR_SERVER "invalid frame, skipping..."


// transcoder.cpp
// --------------

#define STR_TRANSCODER "Transcoder: "
#define STR_TRANSCODER_STARTING	STR_TRANSCODER "starting..."
#define STR_TRANSCODER_FAILED	STR_TRANSCODER "failed!"


// worker.cpp
// ----------

#define STR_WORKER "Worker: "
#define STR_WORKER_PROCESSING STR_WORKER "processing frame..."

#endif // STRINGS_H
