#include "q_shared.h"

const char* RandQuotes[] = {
	"A building has integrity just like a man. And just as seldom.",
	"Civilization is the process of setting man free from men.",
	"Contradictions do not exist. Whenever you think you are facing a contradiction, check your premises. You will find that one of them is wrong.",
	"Do not ever say that the desire to 'do good' by force is a good motive. Neither power-lust nor stupidity are good motives.",
	"Every aspect of Western culture needs a new code of ethics - a rational ethics - as a precondition of rebirth.",
	"Evil requires the sanction of the victim.",
	"Force and mind are opposites; morality ends where a gun begins.",
	"God... a being whose only definition is that he is beyond man's power to conceive.",
	"I don't build in order to have clients. I have clients in order to build.",
	"If any civilization is to survive, it is the morality of altruism that men have to reject.",
	"Individual rights are the means of subordinating society to moral law.",
	"Man's unique reward, however, is that while animals survive by adjusting themselves to their background, man survives by adjusting his background to himself.",
	"Money demands that you sell, not your weakness to men's stupidity, but your talent to their reason.",
	"Money is only a tool. It will take you wherever you wish, but it will not replace you as the driver.",
	"Only the man who does not need it, is fit to inherit wealth, the man who would make his fortune no matter where he started.",
	"Potentially, a government is the most dangerous threat to man's rights: it holds a legal monopoly on the use of physical force against legally disarmed victims.",
	"Reason is not automatic. Those who deny it cannot be conquered by it. Do not count on them. Leave them alone.",
	"Run for your life from any man who tells you that money is evil. That sentence is the leper's bell of an approaching looter.",
	"So you think that money is the root of all evil. Have you ever asked what is the root of all money?",
	"The ladder of success is best climbed by stepping on the rungs of opportunity.",
	"The purpose of morality is to teach you, not to suffer and die, but to enjoy yourself and live.",
	"The truth is not for all men, but only for those who seek it.",
	"The worst guilt is to accept an unearned guilt.",
	"Throughout the centuries there were men who took first steps, down new roads, armed with nothing but their own vision.",
	"The question isn't who is going to let me; it's who is going to stop me.",
	"There is a level of cowardice lower than that of the conformist: the fashionable non-conformist.",
	"To achieve, you need thought. You have to know what you are doing and that's real power.",
	"To say 'I love you' one must first be able to say the 'I.'", 
	"Upper classes are a nation's past; the middle class is its future.",
	"Wealth is the product of man's capacity to think.",
	"When I die, I hope to go to Heaven, whatever the Hell that is.",
	"There are two sides to every issue: one side is right and the other is wrong, but the middle is always evil.",
	"There can be no such thing, in law or in morality, as actions to an individual, but permitted to a mob.",
	"A desire presupposes the possibility of action to achieve it; action presupposes a goal which is worth achieving.",
	"Achieving life is not the equivalent of avoiding death.",
	"Every man builds his world in his own image. He has the power to choose, but no power to escape the necessity of choice.",
	"Happiness is that state of consciousness which proceeds from the achievement of one's values.",
	"I swear, by my life and my love of it, that I will never live for the sake of another man, nor ask another man to live for mine."
};

const char* GetRandQuote() {
	int max = sizeof(RandQuotes) / sizeof(char*);
	int index = Q_irand(0, max);

	return RandQuotes[index];
}