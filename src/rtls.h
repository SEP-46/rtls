#pragma once

#include "communication_interface.h"
#include "config.h"
#include "aabb.h"
#include "tag.h"
#include "trilaterationsolver.h"
#include "velocity_output.h"
#include <memory>
#include <vector>

class RTLS
{
public:
	RTLS();
	~RTLS();

	// Run loop, is called indefinitely until it returns false
	bool Run();

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

private:
	std::unique_ptr<Tag> mTag;
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
	std::vector<std::unique_ptr<CommunicationInterface>> mOutputInterfaces;
	Config mConfig;
	VelocityOutputData mVelocityOutputData;
};