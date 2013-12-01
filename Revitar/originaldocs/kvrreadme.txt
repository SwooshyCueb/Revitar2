http://www.kvraudio.com/forum/viewtopic.php?t=299387

In 2004, I released Revitar 2.0 (guitar VST synth) and greatly enjoyed the community of people that have used and purchased it. However, due to a "real" job, kids, etc. I haven't had time to keep up with it or to answer any support emails (I feel bad about this.)

So I've decided to release a free version of the VST to just say thanks. I'm also releasing the source code for anyone who'd like to play around with it, or build upon it. Everything can be found at http://www.cuttermusic.com. The code should compile using Visual Studio.

If anyone is interested in SawCutter's source code also let me know. This code is very old (coded around 2000) and might take some work to complile.

Thanks again to everyone who has helped me develop these synths, provided presets and beta testers.

[Edited from original post:]

In case anyone was interested in building upon Revitar, I wanted to give my current thoughts on it. First a little background - Revitar produces its sounds by simulating each guitar string using a spring-mass system with 120 points (think 120 metal balls attached to each other with rubber bands). This model allows for a more realistic simulation of the strings, but is a bit harder on the CPU. Cool things like sympathetic resonance (the effect of strings on each other) and different types of damping can be built in. Basically it allows for a richer, and less synthetic sound as compared to other methods like your basic Karplus-Strong approach.

I was happy with Revitar 2.0, but here are some areas I thought could use the most improvement (beyond UI tweaks.)

1. Each string is representing by 120 points. This is enough for a standard guitar, but not enough for a bass guitar. If there aren't enough points the high frequencies aren't reproduced. This is one reason why Revitar doesn't sound as convincing for lower notes. The reason I didn't add more points is the CPU load was getting too high.

2. The guitar body models could use a lot of improvement. I could never get these right. I tried a bunch of different resonance models but nothing seemed to really work (the best I could do is in the code now.) It's hard to tune the guitar body so it sounds rich and warm without any "wolf notes". I really wanted it to have more punch.

3. Guitar strings vibrate in two dimensions, not just one. This could make a difference since I believe the damping due to the bridge varies in each direction. The bridge damping causes the higher frequency harmonics of the string to be slightly out of tune. If these vary in each direction it could give a richer sound - maybe. The string slapping the fret could also create different sounds in each dimension.

4. The pick and string slapping could use some work. Getting the right among of variation is tough, and making sure the right high frequencies come though isn't easy.

5. The code is a bit ugly. A lot of experimentation went into it, and I never properly cleaned it up. Sorry about that.

That's it for now. Good luck if you dig into it.

Regards,

-Larry