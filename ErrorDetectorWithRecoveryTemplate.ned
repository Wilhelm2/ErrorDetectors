package ErrorDetectorWithRecovery;

network ErrorDetectorWithRecovery
{
	@display("bgb=935,338");
	types:
		channel Channel extends ned.DelayChannel
		{
		    delay = 0ms;
		}

	submodules:
