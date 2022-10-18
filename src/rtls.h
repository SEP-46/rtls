#pragma once

#include "communication_interface.h"
#include "config.h"
#include "aabb.h"
#include "tag.h"
#include "trilaterationsolver.h"
#include "velocity_output.h"
#include <memory>
#include <vector>

struct Settings;

class RTLS
{
public:
	RTLS();
	~RTLS();

	void Init( const Settings& settings );

	// Run loop, is called indefinitely until it returns false
	bool Run();
	void Stop();

	std::string GetTagName() const;
	void SetTagName( const std::string& name );

	// Gets configuration for all anchors
	// NOTE: This includes anchors that are not currently connected!
	std::vector<AnchorConfig> GetAnchors() const;
	// Changes anchor name of the given anchor id in the config
	// If the anchor id does not exist, returns false
	bool SetAnchorName( NodeId_t id, const std::string& name );
	// Changes anchor position of the given anchor id in the config
	// If the anchor id does not exist, returns false
	bool SetAnchorPos( NodeId_t id, const Vec3& pos );
	
	void SetBounds( const AABB& bounds );
	const AABB& GetBounds() { return mConfig.bounds; }

private:
	void ApplyConfig( const Config& cfg );
	void SaveConfig();

	AnchorConfig* FindAnchorConfigById( NodeId_t id );
	AnchorConfig* AddAnchorToConfig( NodeId_t id, const std::string& name, const Vec3& pos );

	void SaveLog();

private:
	std::unique_ptr<Tag> mTag;

	struct InputLogEntry
	{
		Timestamp_t timestamp;
		AnchorDistanceMeasurement measurements[MAX_ANCHORS];
		size_t num_measurements;
	};
	std::vector<InputLogEntry> mInputLog;

	struct OutputLogEntry
	{
		Timestamp_t timestamp;
		Vec3 pos;
	};

	struct Algorithm
	{
		std::unique_ptr<ITrilaterationSolver> solver;
		std::vector<std::unique_ptr<CommunicationInterface>> outputs;
		std::vector<OutputLogEntry> log;
	};
	std::vector<Algorithm> mAlgorithms;

	Config mConfig;
	VelocityOutputData mVelocityOutputData;

	bool mShouldLog = false;

	bool mStopped = false;
};