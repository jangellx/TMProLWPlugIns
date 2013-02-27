/*
 * TESTCMD.C -- Modeler Plugin Command
 *		Test requesters and messages
 *
 * written by Stuart Ferguson
 * last revision  12/6/94
 */
#include <splug.h>
#include <lwmod.h>
#include <stdio.h>


	XCALL_(int)
Activate (
	long			 version,
	GlobalFunc		*global,
	ModCommand		*local,
	void			*serverData)
{
	MessageFuncs		*message;
	StateQueryFuncs		*query;
	DynaReqFuncs		*request;
	DynaRequestID		 dr;
	DyReqControlDesc	 desc;
	DynaValue		 val;
	const char		*ohShit;
	char			*strings[4], *items[4];
	char			 t1[40], t2[40], t3[40];
	int			 c1, c2, ok;

	XCALL_INIT;
	if (version != 1)
		return AFUNC_BADVERSION;

	message = (*global) ("Info Messages", GFUSE_TRANSIENT);
	query   = (*global) ("LWM: State Query", GFUSE_TRANSIENT);
	request = (*global) ("LWM: Dynamic Request", GFUSE_TRANSIENT);
	if (!message || !query || !request)
		return AFUNC_BADGLOBAL;

	(*message->warning) ("Hello from TEST COMMAND", "I bring you a warning.");
	(*message->error) ("This would be an error, were not a test.", NULL);

	dr = (*request->create) ("Testing, Testing...");
	if (!dr) {
		ohShit = "Could not create requester.";
		goto fail1;
	}

	ohShit = "Cound not add control.";

	desc.type = DY_TEXT;
	desc.text.text = strings;
	sprintf (t1, "Layers - active:%d  bg:%d",
				(*query->layerMask) (OPLYR_FG),
				(*query->layerMask) (OPLYR_BG));
	sprintf (t2, "Current Surface: %s",
				(*query->surface) ());
	sprintf (t3, "Number of points: %d",
				(*query->bbox) (OPLYR_FG, NULL));
	strings[0] = t1;
	strings[1] = t2;
	strings[2] = t3;
	strings[3] = NULL;
	c1 = (*request->addCtrl) (dr, "Hello", &desc);
	if (c1 < 0)
		goto fail2;

	desc.type = DY_CHOICE;
	desc.choice.vertical = 0;
	desc.choice.items = items;
	items[0] = "Choice 1";
	items[1] = "X";
	items[2] = "Foo and Blah";
	items[3] = NULL;
	c2 = (*request->addCtrl) (dr, "What Next?", &desc);
	if (c2 < 0)
		goto fail2;

	val.type = DY_INTEGER;
	val.intv.value = 1;
	(*request->valueSet) (dr, c2, &val);
	ok = (*request->post) (dr);
	(*request->valueGet) (dr, c2, &val);
	(*request->destroy) (dr);

	if (ok) {
		sprintf (t1, "Choice was %d.", val.intv.value);
		(*message->info) ("Requester was OKed;", t1);
	}
	return AFUNC_OK;

fail2:
	(*request->destroy) (dr);
fail1:
	(*message->error) (ohShit, NULL);
	return AFUNC_OK;
}


/*
 * Globals necessary to declare the class and name of this plugin server.
 */
char		ServerClass[] = "CommandSequence";
char		ServerName[]  = "Demo_TestCommand";
