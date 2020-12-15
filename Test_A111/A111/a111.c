#include "a111.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../Common/acconeer/include/acc_app_integration.h"
#include "../../Common/acconeer/include/acc_definitions.h"
#include "../../Common/acconeer/include/acc_detector_presence.h"
#include "../../Common/acconeer/include/acc_hal_definitions.h"
#include "../../Common/acconeer/include/acc_hal_integration.h"
#include "../../Common/acconeer/include/acc_rss.h"
#include "../../Common/acconeer/include/acc_version.h"

#define DEFAULT_SENSOR_ID            (1)
#define DEFAULT_START_M              (0.18f)
#define DEFAULT_LENGTH_M             (2.00f)
#define DEFAULT_ZONE_LENGTH          (0.4f)
#define DEFAULT_UPDATE_RATE_WAKEUP   (2.0f)
#define DEFAULT_UPDATE_RATE_TRACKING (20.0f)
#define DEFAULT_THRESHOLD            (2.0f)



static void set_default_configuration(acc_detector_presence_configuration_t presence_configuration)
{
	acc_detector_presence_configuration_sensor_set(presence_configuration, DEFAULT_SENSOR_ID);

	acc_detector_presence_configuration_update_rate_set(presence_configuration, DEFAULT_UPDATE_RATE_WAKEUP);
	acc_detector_presence_configuration_detection_threshold_set(presence_configuration, DEFAULT_THRESHOLD);

	acc_detector_presence_configuration_start_set(presence_configuration, DEFAULT_START_M);
	acc_detector_presence_configuration_length_set(presence_configuration, DEFAULT_LENGTH_M);

	acc_detector_presence_configuration_filter_parameters_t filter = acc_detector_presence_configuration_filter_parameters_get(
		presence_configuration);

	filter.output_time_const = 0.0f;
	acc_detector_presence_configuration_filter_parameters_set(presence_configuration, &filter);
}

static bool execute_wakeup(acc_detector_presence_handle_t handle)
{
	acc_detector_presence_result_t result;

	if (!acc_detector_presence_activate(handle))
	{
		fprintf(stderr, "Failed to activate detector\n");
		return false;
	}

	do
	{
		if (!acc_detector_presence_get_next(handle, &result))
		{
			fprintf(stderr, "Failed to get data from sensor\n");
			return false;
		}

		acc_app_integration_sleep_ms(1000 / DEFAULT_UPDATE_RATE_WAKEUP);
	} while (!result.presence_detected);

	uint32_t detected_zone = (uint32_t)((float)(result.presence_distance - DEFAULT_START_M) / (float)DEFAULT_ZONE_LENGTH);
	printf("Motion in zone: %u, distance: %d, score: %d\n", (unsigned int)detected_zone, (int)(result.presence_distance * 1000.0f),
	       (int)(result.presence_score * 1000.0f));

	acc_detector_presence_deactivate(handle);

	return true;
}

static bool execute_movement_tracking(acc_detector_presence_handle_t handle)
{
	acc_detector_presence_result_t result;

	if (!acc_detector_presence_activate(handle))
	{
		fprintf(stderr, "Failed to activate detector\n");
		return false;
	}

	do
	{
		if (!acc_detector_presence_get_next(handle, &result))
		{
			fprintf(stderr, "Failed to get data from sensor\n");
			return false;
		}

		if (result.presence_detected)
		{
			uint32_t detected_zone = (uint32_t)((float)(result.presence_distance - DEFAULT_START_M) / (float)DEFAULT_ZONE_LENGTH);
			printf("Motion in zone: %u, distance: %d, score: %d\n", (unsigned int)detected_zone,
			       (int)(result.presence_distance * 1000.0f),
			       (int)(result.presence_score * 1000.0f));
		}

		acc_app_integration_sleep_ms(1000 / DEFAULT_UPDATE_RATE_TRACKING);
	} while (result.presence_detected);

	printf("No motion, score: %d\n", (int)(result.presence_score * 1000.0f));

	acc_detector_presence_deactivate(handle);

	return true;
}

bool acconeer_example(void)
{
	printf("Acconeer software version %s\n", acc_version_get());

	const acc_hal_t *hal = acc_hal_integration_get_implementation();

	if (!acc_rss_activate(hal))
	{
		fprintf(stderr, "Failed to activate RSS\n");
		return false;
	}

	acc_detector_presence_configuration_t presence_configuration = acc_detector_presence_configuration_create();
	if (presence_configuration == NULL)
	{
		fprintf(stderr, "Failed to create configuration\n");
		acc_rss_deactivate();
		return false;
	}

	set_default_configuration(presence_configuration);
	acc_detector_presence_configuration_power_save_mode_set(presence_configuration, ACC_POWER_SAVE_MODE_OFF);

	acc_detector_presence_handle_t handle = acc_detector_presence_create(presence_configuration);
	if (handle == NULL)
	{
		fprintf(stderr, "Failed to create detector\n");
		acc_detector_presence_configuration_destroy(&presence_configuration);
		acc_detector_presence_destroy(&handle);
		acc_rss_deactivate();
		return false;
	}

	while (true)
	{
		if (!execute_wakeup(handle))
		{
			acc_detector_presence_configuration_destroy(&presence_configuration);
			acc_detector_presence_destroy(&handle);
			acc_rss_deactivate();
			return false;
		}

		acc_detector_presence_configuration_update_rate_set(presence_configuration, DEFAULT_UPDATE_RATE_TRACKING);
		acc_detector_presence_configuration_power_save_mode_set(presence_configuration, ACC_POWER_SAVE_MODE_SLEEP);

		if (!acc_detector_presence_reconfigure(&handle, presence_configuration))
		{
			fprintf(stderr, "Failed to reconfigure detector\n");
			acc_detector_presence_configuration_destroy(&presence_configuration);
			acc_detector_presence_destroy(&handle);
			acc_rss_deactivate();
			return false;
		}

		if (!execute_movement_tracking(handle))
		{
			acc_detector_presence_configuration_destroy(&presence_configuration);
			acc_detector_presence_destroy(&handle);
			acc_rss_deactivate();
			return false;
		}

		acc_detector_presence_configuration_update_rate_set(presence_configuration, DEFAULT_UPDATE_RATE_WAKEUP);
		acc_detector_presence_configuration_power_save_mode_set(presence_configuration, ACC_POWER_SAVE_MODE_OFF);

		if (!acc_detector_presence_reconfigure(&handle, presence_configuration))
		{
			fprintf(stderr, "Failed to reconfigure detector\n");
			acc_detector_presence_configuration_destroy(&presence_configuration);
			acc_detector_presence_destroy(&handle);
			acc_rss_deactivate();
			return false;
		}
	}

	// We will never exit so no need to destroy the configuration or detector

	return true;
}