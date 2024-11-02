
#include "cpucounters.h"

using namespace pcm;

extern "C" {

	std::shared_ptr<ServerUncorePMUs>  serverUncorePMUs;
	ServerUncorePMUs *caladan_cfg;
	PCM *cpcm;

	uint32_t pcm_caladan_get_cas_count(uint32 channel)
	{
		return caladan_cfg->getMCCounter(channel, ServerUncorePMUs::EventPosition::READ);
	}

	uint32_t pcm_caladan_get_active_channel_count(void)
	{
		uint32_t count = 0;
		std::vector<uint32_t> cnts;
		for (uint32_t i = 0; i < cpcm->getMCChannelsPerSocket(); i++) {
			cnts.push_back(pcm_caladan_get_cas_count(i));
		}
		usleep(10000);
		for (uint32_t i = 0; i < cpcm->getMCChannelsPerSocket(); i++) {
			count += pcm_caladan_get_cas_count(i) != cnts[i];
		}

		return count;
	}

	int pcm_caladan_init(int socket)
	{
		cpcm = PCM::getInstance();
		serverUncorePMUs = cpcm->serverUncorePMUs.at(socket);
		caladan_cfg = serverUncorePMUs.get();
		caladan_cfg->programServerUncoreMemoryMetrics(PartialWrites, -1, -1);
		return 0;
	}
}
