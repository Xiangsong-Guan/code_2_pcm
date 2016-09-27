#include <defines.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*optional parameters*/
enum opt_channels opt_ch;
enum opt_sample_ps opt_sps;
enum opt_bits_per_sample opt_bps;

/*necessary parameters*/
static const char *i_path;
static const char *o_path;

/*file*/
FILE *i_file;
FILE *o_file;

static void usage()
{
	puts("---------- Usage for 0.1.0 ver. | writen by Xiangsong Guan ----------");
	puts("Encode your source code file to a pcm codec wave file, just for fun.");
	puts("code_2_pcm [options] input_file_path output_file_path");
	puts("options:");
	puts("  -h  print this usage.");
	puts("  -c  decide the number of Sound Channel.");
	puts("      m(ono)      -> single sound channel.");
	puts("      s(tereo)    -> output file will contain left & right sound channel.");
	puts("      [default]when miss this option, the default value 'stereo' will be applied.");
	puts("  -s  decide the sample per second.");
	puts("      t(elephone) -> 8000Hz which is used by Telecommunication.");
	puts("      r(adio)     -> 22050Hz which is used by Radio broadcasting.");
	puts("      c(d)        -> 44100Hz which is used by CD quality.");
	puts("      d(vd)       -> 48000Hz which is used by High quality DVD and Digital TV.");
	puts("      [default]when miss this option, the default value 'cd' will be applied.");
	puts("  -b  decide the bits per sample.");
	puts("      l(ow)       -> 8bits which make low Fidelity.");
	puts("      n(omal)     -> 16bits which is widly used in music & video.");
	puts("      h(igh)      -> 24bits which is used to make high Fidelity.");
	puts("      [default]when miss this option, the default value 'nomal' will be applied.");
	return;
}

/*check the cmd is valid or not, return 0 if no problems. self print.*/
static int parameter_check(const int parameters_cnt, const char **parameters)
{
	/*verify cmd via number of parameters*/
	if(parameters_cnt < 3)
	{
		if(parameters[1][1] != 'h')
		{
			puts("Incorrect parameters!");
			usage();
			return 1;
		}
		else
		{
			usage();
			return 0;
		}
	}

	i_path = parameters[parameters_cnt - 2];
	o_path = parameters[parameters_cnt - 1];

	/*input file is readble?*/
	i_file = fopen(i_path, "rt");
	if(i_file == NULL)
	{
		perror("Input file: ");
		return 1;
	}

	/*output file is already existed? writable?*/
	o_file = fopen(o_path, "rb");
	if(o_file != NULL)
	{
		fclose(o_file);
		puts("Output file is already existed! Overwrite? [y/n]");
		/* debug
		if(getchar() != 'y')
		{
			fclose(i_file);
			puts("Abort.");
			return 1;
		}
		*/
		puts("debug: y");
	}
	o_file = fopen(o_path, "wb");
	if(o_file == NULL)
	{
		perror("Output file: ");
		fclose(i_file);
		return 1;
	}

	return 0;
}

/*set the options according to cmd parameters, return 0 if no problems. self print.*/
int interpret_parameter(const int parameters_cnt, const char **parameters)
{
	const char *pc;
	char opt;
	int i;

	opt_ch = stereo;
	opt_sps = cd;
	opt_bps = nomal;

	opt = 0x00;

	if(parameter_check(parameters_cnt, parameters) != 0)
	{
		return 1;
	}

	/*interpret parameters*/
	for(i = 1; i <= (parameters_cnt - 3); i++)
	{
		pc = parameters[i];

		if(opt == 0x00)
		{
			if(pc[0] == '-')
			{
				switch(pc[1])
				{
					case HELP:
					case CHANNELS:
					case BITS_PER_SAMPLE:
					case SAMPLE_PS:
						opt = pc[1];
						break;
					default:
						puts("ERROR! Invalid option.");
						usage();
						return 1;
				}
			}
			else
			{
				puts("ERROR! Invalid parameters!");
				usage();
				return 1;
			}
		}
		else
		{
			switch(opt)
			{
				case HELP:
					puts("Uninvalid input to -h! There should not with anything else!");
					usage();
					return 1;
				case CHANNELS:
					switch(pc[0])
					{
						case OPT_STEREO:
							opt_ch = stereo;
							break;
						case OPT_MONO:
							opt_ch = mono;
							break;
						default:
							puts("Wrong channels option!");
							usage();
							return 1;
					}
					break;
				case SAMPLE_PS:
					switch(pc[0])
					{
						case OPT_TELEPHONE:
							opt_sps = telephone;
							break;
						case OPT_RADIO:
							opt_sps = radio;
							break;
						case OPT_CD:
							opt_sps = cd;
							break;
						case OPT_DVD:
							opt_sps = dvd;
							break;
						default:
							puts("Wrong sample per second option!");
							usage();
							return 1;
					}
					break;
				case BITS_PER_SAMPLE:
					switch(pc[0])
					{
						case OPT_LOW:
							opt_bps = low;
							break;
						case OPT_NOMAL:
							opt_bps = nomal;
							break;
						case OPT_HIGH:
							opt_bps = high;
							break;
						default:
							puts("Wrong bits per sample option!");
							usage();
							return 1;
					}
				default:
					puts("Wrong option!");
					usage();
					return 1;
			}
			opt = 0x00;
		}
	}

	if(opt != 0x00)
	{
		puts("ERROR! Lack of some parameters!");
		usage();
		return 1;
	}
	return 0;
}