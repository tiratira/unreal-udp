// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerFrame.h"

FThirdPlayerInput::FThirdPlayerInput(CameraAxisInput* input)
{
	cameraYaw = input->camerayaw();
	moveForward = input->moveforward();
	moveRight = input->moveright();
	jump = input->jump();
}




FClientFrame::FClientFrame(ClientFrame* clientFrame) {
	clientId = clientFrame->clientid();
	input = FThirdPlayerInput(clientFrame->mutable_input());
}



FServerFrame::FServerFrame(ServerFrame* serverFrame) {
	frameId = serverFrame->frameid();
	TArray<FClientFrame> _clientFrames;

	for (int i = 0; i < serverFrame->clientinputs_size(); i++)
	{
		const ClientFrame& frame_item = serverFrame->clientinputs(i);
		const CameraAxisInput frame_input = frame_item.input();

		FClientFrame frame_in;
		frame_in.clientId = frame_item.clientid();
		frame_in.input.cameraYaw = frame_input.camerayaw();
		frame_in.input.moveForward = frame_input.moveforward();
		frame_in.input.moveRight = frame_input.moveright();
		frame_in.input.jump = frame_input.jump();
		_clientFrames.Add(frame_in);
	}
	clientFrames = _clientFrames;
}

//ServerFrame FServerFrame::ToProto() {
//	ServerFrame frame;
//	frame.set_frameid(this->frameId);
//}

FServerFramePackage::FServerFramePackage(ServerFramePackage* package) {
	current = FServerFrame(package->mutable_current());
	cache1 = FServerFrame(package->mutable_cache1());
	cache2 = FServerFrame(package->mutable_cache2());
}

//ServerFramePackage FServerFramePackage::ToProto() {
//
//}
