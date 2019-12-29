CXX           = g++
CXXFLAGS      = -O3 -Wall -Wno-sign-compare

#note: use "pkg-config libxml++-2.6 --cflags --libs" to determine includes and libs for libxml++

#32 bit
#INCPATH       = -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

#64 bit
INCPATH       = -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib64/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include

LIBS          = -lgsl -lgslcblas -lm -lxml++-2.6 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0

LINK          = g++
LFLAGS        =
DEL_FILE      = rm -f

TARGET        = XMBF

SOURCES       = main.cpp \
		combined_model.cpp \
		gaussian_prior.cpp \
		chisqr_extra_term.cpp \
		fitter.cpp \
		binary_io.cpp \
		load_data_file.cpp \
		xml_tools.cpp \
		command_line_parser.cpp \
		replace_macros.cpp \
		get_fit_settings.cpp \
		create_models.cpp \
		parser.cpp \
		prepare_fit_data.cpp \
		get_constants.cpp \
		get_chi_sqr_extra_term.cpp \
		get_parameters.cpp \
		fit.cpp \
		write_fit_results.cpp \
		plot.cpp \
		bootstrap.cpp \
		multifit.cpp \
		string_tools.cpp \
		parse_model.cpp \
		multi_exp_model.cpp \
		multi_exp_expE_model.cpp \
		multi_exp_Asqr_model.cpp \
		multi_exp_Asqr_expE_model.cpp \
		multi_alt_exp_model.cpp \
		multi_alt_exp_expE_model.cpp \
		multi_alt_exp_Asqr_model.cpp \
		multi_alt_exp_Asqr_expE_model.cpp \
		multi_exp_vec_model.cpp \
		multi_exp_expE_vec_model.cpp \
		multi_exp_Asqr_vec_model.cpp \
		multi_exp_Asqr_expE_vec_model.cpp \
		multi_alt_exp_vec_model.cpp \
		multi_alt_exp_expE_vec_model.cpp \
		multi_alt_exp_Asqr_vec_model.cpp \
		multi_alt_exp_Asqr_expE_vec_model.cpp \
		multi_exp_BC_model.cpp \
		multi_exp_expE_BC_model.cpp \
		multi_exp_Asqr_BC_model.cpp \
		multi_exp_Asqr_expE_BC_model.cpp \
		multi_alt_exp_BC_model.cpp \
		multi_alt_exp_expE_BC_model.cpp \
		multi_alt_exp_Asqr_BC_model.cpp \
		multi_alt_exp_Asqr_expE_BC_model.cpp \
		multi_exp_vec_BC_model.cpp \
		multi_exp_expE_vec_BC_model.cpp \
		multi_exp_Asqr_vec_BC_model.cpp \
		multi_exp_Asqr_expE_vec_BC_model.cpp \
		multi_alt_exp_vec_BC_model.cpp \
		multi_alt_exp_expE_vec_BC_model.cpp \
		multi_alt_exp_Asqr_vec_BC_model.cpp \
		multi_alt_exp_Asqr_expE_vec_BC_model.cpp \
		multi_exp_const_model.cpp \
		multi_exp_expE_const_model.cpp \
		multi_exp_Asqr_const_model.cpp \
		multi_exp_Asqr_expE_const_model.cpp \
		multi_alt_exp_const_model.cpp \
		multi_alt_exp_expE_const_model.cpp \
		multi_alt_exp_Asqr_const_model.cpp \
		multi_alt_exp_Asqr_expE_const_model.cpp \
		multi_exp_vec_const_model.cpp \
		multi_exp_expE_vec_const_model.cpp \
		multi_exp_Asqr_vec_const_model.cpp \
		multi_exp_Asqr_expE_vec_const_model.cpp \
		multi_alt_exp_vec_const_model.cpp \
		multi_alt_exp_expE_vec_const_model.cpp \
		multi_alt_exp_Asqr_vec_const_model.cpp \
		multi_alt_exp_Asqr_expE_vec_const_model.cpp \
		multi_exp_BC_const_model.cpp \
		multi_exp_expE_BC_const_model.cpp \
		multi_exp_Asqr_BC_const_model.cpp \
		multi_exp_Asqr_expE_BC_const_model.cpp \
		multi_alt_exp_BC_const_model.cpp \
		multi_alt_exp_expE_BC_const_model.cpp \
		multi_alt_exp_Asqr_BC_const_model.cpp \
		multi_alt_exp_Asqr_expE_BC_const_model.cpp \
		multi_exp_vec_BC_const_model.cpp \
		multi_exp_expE_vec_BC_const_model.cpp \
		multi_exp_Asqr_vec_BC_const_model.cpp \
		multi_exp_Asqr_expE_vec_BC_const_model.cpp \
		multi_alt_exp_vec_BC_const_model.cpp \
		multi_alt_exp_expE_vec_BC_const_model.cpp \
		multi_alt_exp_Asqr_vec_BC_const_model.cpp \
		multi_alt_exp_Asqr_expE_vec_BC_const_model.cpp \
		multi_exp_mat_model.cpp \
		multi_exp_mat_upper_model.cpp \
		multi_exp_mat_II_model.cpp \
		multi_exp_mat_II_upper_model.cpp \
		multi_exp_expE_mat_model.cpp \
		multi_exp_expE_mat_upper_model.cpp \
		multi_exp_expE_mat_II_model.cpp \
		multi_exp_expE_mat_II_upper_model.cpp \
		multi_alt_exp_mat_model.cpp \
		multi_alt_exp_expE_mat_model.cpp \
		multi_exp_nonsym_mat_model.cpp \
		multi_exp_expE_nonsym_mat_model.cpp \
		multi_alt_exp_nonsym_mat_model.cpp \
		multi_alt_exp_expE_nonsym_mat_model.cpp \
		multi_exp_2var_model.cpp \
		multi_exp_expE_2var_model.cpp \
		threept_constant_model.cpp \
		threept_constant_sqr_model.cpp \
		threept_multi_exp_model.cpp \
		threept_multi_exp_expE_model.cpp \
		threept_multi_alt_exp_model.cpp \
		threept_multi_alt_exp_expE_model.cpp \
		threept_multi_exp_vec_model.cpp \
		threept_multi_exp_expE_vec_model.cpp \
		threept_multi_alt_exp_vec_model.cpp \
		threept_multi_alt_exp_expE_vec_model.cpp \
		multi_part_exp_expE_model.cpp 


OBJECTS       = main.o \
		combined_model.o \
		gaussian_prior.o \
		chisqr_extra_term.o \
		fitter.o \
		binary_io.o \
		load_data_file.o \
		xml_tools.o \
		command_line_parser.o \
		replace_macros.o \
		get_fit_settings.o \
		create_models.o \
		parser.o \
		prepare_fit_data.o \
		get_constants.o \
		get_chi_sqr_extra_term.o \
		get_parameters.o \
		fit.o \
		write_fit_results.o \
		plot.o \
		bootstrap.o \
		multifit.o \
		string_tools.o \
		parse_model.o \
		multi_exp_model.o \
		multi_exp_expE_model.o \
		multi_exp_Asqr_model.o \
		multi_exp_Asqr_expE_model.o \
		multi_alt_exp_model.o \
		multi_alt_exp_expE_model.o \
		multi_alt_exp_Asqr_model.o \
		multi_alt_exp_Asqr_expE_model.o \
		multi_exp_vec_model.o \
		multi_exp_expE_vec_model.o \
		multi_exp_Asqr_vec_model.o \
		multi_exp_Asqr_expE_vec_model.o \
		multi_alt_exp_vec_model.o \
		multi_alt_exp_expE_vec_model.o \
		multi_alt_exp_Asqr_vec_model.o \
		multi_alt_exp_Asqr_expE_vec_model.o \
		multi_exp_BC_model.o \
		multi_exp_expE_BC_model.o \
		multi_exp_Asqr_BC_model.o \
		multi_exp_Asqr_expE_BC_model.o \
		multi_alt_exp_BC_model.o \
		multi_alt_exp_expE_BC_model.o \
		multi_alt_exp_Asqr_BC_model.o \
		multi_alt_exp_Asqr_expE_BC_model.o \
		multi_exp_vec_BC_model.o \
		multi_exp_expE_vec_BC_model.o \
		multi_exp_Asqr_vec_BC_model.o\
		multi_exp_Asqr_expE_vec_BC_model.o \
		multi_alt_exp_vec_BC_model.o \
		multi_alt_exp_expE_vec_BC_model.o \
		multi_alt_exp_Asqr_vec_BC_model.o \
		multi_alt_exp_Asqr_expE_vec_BC_model.o \
		multi_exp_const_model.o \
		multi_exp_expE_const_model.o \
		multi_exp_Asqr_const_model.o \
		multi_exp_Asqr_expE_const_model.o \
		multi_alt_exp_const_model.o \
		multi_alt_exp_expE_const_model.o \
		multi_alt_exp_Asqr_const_model.o \
		multi_alt_exp_Asqr_expE_const_model.o \
		multi_exp_vec_const_model.o \
		multi_exp_expE_vec_const_model.o \
		multi_exp_Asqr_vec_const_model.o \
		multi_exp_Asqr_expE_vec_const_model.o \
		multi_alt_exp_vec_const_model.o \
		multi_alt_exp_expE_vec_const_model.o \
		multi_alt_exp_Asqr_vec_const_model.o \
		multi_alt_exp_Asqr_expE_vec_const_model.o \
		multi_exp_BC_const_model.o \
		multi_exp_expE_BC_const_model.o \
		multi_exp_Asqr_BC_const_model.o \
		multi_exp_Asqr_expE_BC_const_model.o \
		multi_alt_exp_BC_const_model.o \
		multi_alt_exp_expE_BC_const_model.o \
		multi_alt_exp_Asqr_BC_const_model.o \
		multi_alt_exp_Asqr_expE_BC_const_model.o \
		multi_exp_vec_BC_const_model.o \
		multi_exp_expE_vec_BC_const_model.o \
		multi_exp_Asqr_vec_BC_const_model.o \
		multi_exp_Asqr_expE_vec_BC_const_model.o \
		multi_alt_exp_vec_BC_const_model.o \
		multi_alt_exp_expE_vec_BC_const_model.o \
		multi_alt_exp_Asqr_vec_BC_const_model.o \
		multi_alt_exp_Asqr_expE_vec_BC_const_model.o \
		multi_exp_mat_model.o \
		multi_exp_mat_upper_model.o \
		multi_exp_mat_II_model.o \
		multi_exp_mat_II_upper_model.o \
		multi_exp_expE_mat_model.o \
		multi_exp_expE_mat_upper_model.o \
		multi_exp_expE_mat_II_model.o \
		multi_exp_expE_mat_II_upper_model.o \
		multi_alt_exp_mat_model.o \
		multi_alt_exp_expE_mat_model.o \
		multi_exp_nonsym_mat_model.o \
		multi_exp_expE_nonsym_mat_model.o \
		multi_alt_exp_nonsym_mat_model.o \
		multi_alt_exp_expE_nonsym_mat_model.o \
		multi_exp_2var_model.o \
		multi_exp_expE_2var_model.o \
		threept_constant_model.o \
		threept_constant_sqr_model.o \
		threept_multi_exp_model.o \
		threept_multi_exp_expE_model.o \
		threept_multi_alt_exp_model.o \
		threept_multi_alt_exp_expE_model.o \
		threept_multi_exp_vec_model.o \
		threept_multi_exp_expE_vec_model.o \
		threept_multi_alt_exp_vec_model.o \
		threept_multi_alt_exp_expE_vec_model.o \
		multi_part_exp_expE_model.o


$(TARGET):  $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)


main.o: 	main.cpp \
		fitter.h \
		abstract_model.h \
		abstract_prior.h \
		gaussian_prior.h \
		chisqr_extra_term.h \
		combined_model.h \
		load_data_file.h \
		command_line_parser.h \
		replace_macros.h \
		get_fit_settings.h \
		create_models.h \
		get_constants.h \
		get_chi_sqr_extra_term.h \
		get_parameters.h \
		prepare_fit_data.h \
		fit.h \
		write_fit_results.h \
		plot.h \
		bootstrap.h \
		multifit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

create_models.o: create_models.cpp \
		xml_tools.h \
		load_data_file.h \
		abstract_model.h \
		parse_model.h \
		multi_exp_model.h \
		multi_exp_expE_model.h \
		multi_exp_Asqr_model.h \
		multi_exp_Asqr_expE_model.h \
		multi_alt_exp_model.h \
		multi_alt_exp_expE_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_vec_model.h \
		multi_exp_expE_vec_model.h \
		multi_exp_Asqr_vec_model.h \
		multi_exp_Asqr_expE_vec_model.h \
		multi_alt_exp_vec_model.h \
		multi_alt_exp_expE_vec_model.h \
		multi_alt_exp_Asqr_vec_model.h \
		multi_alt_exp_Asqr_expE_vec_model.h \
		multi_exp_BC_model.h \
		multi_exp_expE_BC_model.h \
		multi_exp_Asqr_BC_model.h \
		multi_exp_Asqr_expE_BC_model.h \
		multi_alt_exp_BC_model.h \
		multi_alt_exp_expE_BC_model.h \
		multi_alt_exp_Asqr_BC_model.h \
		multi_alt_exp_Asqr_expE_BC_model.h \
		multi_exp_vec_BC_model.h \
		multi_exp_expE_vec_BC_model.h \
		multi_exp_Asqr_vec_BC_model.h \
		multi_exp_Asqr_expE_vec_BC_model.h \
		multi_alt_exp_vec_BC_model.h \
		multi_alt_exp_expE_vec_BC_model.h \
		multi_alt_exp_Asqr_vec_BC_model.h \
		multi_alt_exp_Asqr_expE_vec_BC_model.h \
		multi_exp_mat_model.h \
		multi_exp_mat_upper_model.h \
		multi_exp_mat_II_model.h \
		multi_exp_mat_II_upper_model.h \
		multi_exp_expE_mat_model.h \
		multi_exp_expE_mat_upper_model.h \
		multi_exp_expE_mat_II_model.h \
		multi_exp_expE_mat_II_upper_model.h \
		multi_alt_exp_mat_model.h \
		multi_alt_exp_expE_mat_model.h \
		multi_exp_nonsym_mat_model.h \
		multi_exp_expE_nonsym_mat_model.h \
		multi_alt_exp_nonsym_mat_model.h \
		multi_alt_exp_expE_nonsym_mat_model.h \
		multi_exp_2var_model.h \
		multi_exp_expE_2var_model.h \
		multi_exp_const_model.h \
		multi_exp_expE_const_model.h \
		multi_exp_Asqr_const_model.h \
		multi_exp_Asqr_expE_const_model.h \
		multi_alt_exp_const_model.h \
		multi_alt_exp_expE_const_model.h \
		multi_alt_exp_Asqr_const_model.h \
		multi_alt_exp_Asqr_expE_const_model.h \
		multi_exp_vec_const_model.h \
		multi_exp_expE_vec_const_model.h \
		multi_exp_Asqr_vec_const_model.h \
		multi_exp_Asqr_expE_vec_const_model.h \
		multi_alt_exp_vec_const_model.h \
		multi_alt_exp_expE_vec_const_model.h \
		multi_alt_exp_Asqr_vec_const_model.h \
		multi_alt_exp_Asqr_expE_vec_const_model.h \
		multi_exp_BC_const_model.h \
		multi_exp_expE_BC_const_model.h \
		multi_exp_Asqr_BC_const_model.h \
		multi_exp_Asqr_expE_BC_const_model.h \
		multi_alt_exp_BC_const_model.h \
		multi_alt_exp_expE_BC_const_model.h \
		multi_alt_exp_Asqr_BC_const_model.h \
		multi_alt_exp_Asqr_expE_BC_const_model.h \
		multi_exp_vec_BC_const_model.h \
		multi_exp_expE_vec_BC_const_model.h \
		multi_exp_Asqr_vec_BC_const_model.h \
		multi_exp_Asqr_expE_vec_BC_const_model.h \
		multi_alt_exp_vec_BC_const_model.h \
		multi_alt_exp_expE_vec_BC_const_model.h \
		multi_alt_exp_Asqr_vec_BC_const_model.h \
		multi_alt_exp_Asqr_expE_vec_BC_const_model.h \
		threept_constant_model.h \
		threept_constant_sqr_model.h \
		threept_multi_exp_model.h \
		threept_multi_exp_expE_model.h \
		threept_multi_alt_exp_model.h \
		threept_multi_alt_exp_expE_model.h \
		threept_multi_exp_vec_model.h \
		threept_multi_exp_expE_vec_model.h \
		threept_multi_alt_exp_vec_model.h \
		threept_multi_alt_exp_expE_vec_model.h \
		multi_part_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o create_models.o create_models.cpp

prepare_fit_data.o: prepare_fit_data.cpp prepare_fit_data.h parser.h abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o prepare_fit_data.o prepare_fit_data.cpp

plot.o: plot.cpp plot.h parser.h abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o plot.o plot.cpp

string_tools.o: string_tools.cpp string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o string_tools.o string_tools.cpp

fit.o: fit.cpp fit.h fitter.h gaussian_prior.h string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fit.o fit.cpp

write_fit_results.o: write_fit_results.cpp write_fit_results.h fitter.h string_tools.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o write_fit_results.o write_fit_results.cpp

bootstrap.o: bootstrap.cpp bootstrap.h fitter.h gaussian_prior.h string_tools.h prepare_fit_data.h abstract_model.h combined_model.h parser.h chisqr_extra_term.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o bootstrap.o bootstrap.cpp

multifit.o: multifit.cpp multifit.h fitter.h gaussian_prior.h string_tools.h prepare_fit_data.h abstract_model.h combined_model.h parser.h chisqr_extra_term.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multifit.o multifit.cpp

replace_macros.o: replace_macros.cpp replace_macros.h string_tools.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o replace_macros.o replace_macros.cpp

get_fit_settings.o: get_fit_settings.cpp get_fit_settings.h fitter.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o get_fit_settings.o get_fit_settings.cpp

get_constants.o: get_constants.cpp get_constants.h combined_model.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o get_constants.o get_constants.cpp

get_chi_sqr_extra_term.o: get_chi_sqr_extra_term.cpp get_chi_sqr_extra_term.h combined_model.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o get_chi_sqr_extra_term.o get_chi_sqr_extra_term.cpp

get_parameters.o: get_parameters.cpp get_parameters.h combined_model.h xml_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o get_parameters.o get_parameters.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parser.o parser.cpp

binary_io.o: binary_io.cpp binary_io.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o binary_io.o binary_io.cpp

load_data_file.o: load_data_file.cpp load_data_file.h \
		 binary_io.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o load_data_file.o load_data_file.cpp

xml_tools.o: xml_tools.cpp xml_tools.h string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o xml_tools.o xml_tools.cpp

command_line_parser.o: command_line_parser.cpp command_line_parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o command_line_parser.o command_line_parser.cpp

gaussian_prior.o: gaussian_prior.cpp gaussian_prior.h \
		abstract_prior.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gaussian_prior.o gaussian_prior.cpp

chisqr_extra_term.o: chisqr_extra_term.cpp chisqr_extra_term.h \
		abstract_prior.h \
		parser.h \
		string_tools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o chisqr_extra_term.o chisqr_extra_term.cpp

fitter.o: fitter.cpp fitter.h \
		abstract_global_model.h \
		abstract_prior.h \
		chisqr_extra_term.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fitter.o fitter.cpp

combined_model.o: combined_model.cpp combined_model.h \
		abstract_global_model.h abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o combined_model.o combined_model.cpp

parse_model.o: parse_model.cpp parse_model.h \
		abstract_model.h \
		parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parse_model.o parse_model.cpp

multi_exp_model.o: multi_exp_model.cpp multi_exp_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_model.o multi_exp_model.cpp

multi_exp_expE_model.o: multi_exp_expE_model.cpp multi_exp_expE_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_model.o multi_exp_expE_model.cpp

multi_exp_Asqr_model.o: multi_exp_Asqr_model.cpp multi_exp_Asqr_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_model.o multi_exp_Asqr_model.cpp

multi_exp_Asqr_expE_model.o: multi_exp_Asqr_expE_model.cpp multi_exp_Asqr_expE_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_model.o multi_exp_Asqr_expE_model.cpp

multi_alt_exp_model.o: multi_alt_exp_model.cpp multi_alt_exp_model.h \
		abstract_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_model.o multi_alt_exp_model.cpp

multi_alt_exp_expE_model.o: multi_alt_exp_expE_model.cpp multi_alt_exp_expE_model.h \
		abstract_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_model.o multi_alt_exp_expE_model.cpp

multi_alt_exp_Asqr_model.o: multi_alt_exp_Asqr_model.cpp multi_alt_exp_Asqr_model.h \
		abstract_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_model.o multi_alt_exp_Asqr_model.cpp

multi_alt_exp_Asqr_expE_model.o: multi_alt_exp_Asqr_expE_model.cpp multi_alt_exp_Asqr_expE_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_model.o multi_alt_exp_Asqr_expE_model.cpp

multi_exp_vec_model.o: multi_exp_vec_model.cpp multi_exp_vec_model.h \
		abstract_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_vec_model.o multi_exp_vec_model.cpp

multi_exp_expE_vec_model.o: multi_exp_expE_vec_model.cpp multi_exp_expE_vec_model.h \
		abstract_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_vec_model.o multi_exp_expE_vec_model.cpp

multi_exp_Asqr_vec_model.o: multi_exp_Asqr_vec_model.cpp multi_exp_Asqr_vec_model.h \
		abstract_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_vec_model.o multi_exp_Asqr_vec_model.cpp

multi_exp_Asqr_expE_vec_model.o: multi_exp_Asqr_expE_vec_model.cpp multi_exp_Asqr_expE_vec_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_vec_model.o multi_exp_Asqr_expE_vec_model.cpp

multi_alt_exp_vec_model.o: multi_alt_exp_vec_model.cpp multi_alt_exp_vec_model.h \
		abstract_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_vec_model.o multi_alt_exp_vec_model.cpp

multi_alt_exp_expE_vec_model.o: multi_alt_exp_expE_vec_model.cpp multi_alt_exp_expE_vec_model.h \
		abstract_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_vec_model.o multi_alt_exp_expE_vec_model.cpp

multi_alt_exp_Asqr_vec_model.o: multi_alt_exp_Asqr_vec_model.cpp multi_alt_exp_Asqr_vec_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_vec_model.o multi_alt_exp_Asqr_vec_model.cpp

multi_alt_exp_Asqr_expE_vec_model.o: multi_alt_exp_Asqr_expE_vec_model.cpp multi_alt_exp_Asqr_expE_vec_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_vec_model.o multi_alt_exp_Asqr_expE_vec_model.cpp

multi_exp_BC_model.o: multi_exp_BC_model.cpp multi_exp_BC_model.h \
		abstract_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_BC_model.o multi_exp_BC_model.cpp

multi_exp_expE_BC_model.o: multi_exp_expE_BC_model.cpp multi_exp_expE_BC_model.h \
		abstract_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_BC_model.o multi_exp_expE_BC_model.cpp

multi_exp_Asqr_BC_model.o: multi_exp_Asqr_BC_model.cpp multi_exp_Asqr_BC_model.h \
		abstract_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_BC_model.o multi_exp_Asqr_BC_model.cpp

multi_exp_Asqr_expE_BC_model.o: multi_exp_Asqr_expE_BC_model.cpp multi_exp_Asqr_expE_BC_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_BC_model.o multi_exp_Asqr_expE_BC_model.cpp

multi_alt_exp_BC_model.o: multi_alt_exp_BC_model.cpp multi_alt_exp_BC_model.h \
		abstract_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_BC_model.o multi_alt_exp_BC_model.cpp

multi_alt_exp_expE_BC_model.o: multi_alt_exp_expE_BC_model.cpp multi_alt_exp_expE_BC_model.h \
		abstract_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_BC_model.o multi_alt_exp_expE_BC_model.cpp

multi_alt_exp_Asqr_BC_model.o: multi_alt_exp_Asqr_BC_model.cpp multi_alt_exp_Asqr_BC_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_BC_model.o multi_alt_exp_Asqr_BC_model.cpp

multi_alt_exp_Asqr_expE_BC_model.o: multi_alt_exp_Asqr_expE_BC_model.cpp multi_alt_exp_Asqr_expE_BC_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_BC_model.o multi_alt_exp_Asqr_expE_BC_model.cpp

multi_exp_vec_BC_model.o: multi_exp_vec_BC_model.cpp multi_exp_vec_BC_model.h \
		abstract_model.h \
		multi_exp_vec_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_vec_BC_model.o multi_exp_vec_BC_model.cpp

multi_exp_expE_vec_BC_model.o: multi_exp_expE_vec_BC_model.cpp multi_exp_expE_vec_BC_model.h \
		abstract_model.h \
		multi_exp_expE_vec_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_vec_BC_model.o multi_exp_expE_vec_BC_model.cpp

multi_exp_Asqr_vec_BC_model.o: multi_exp_Asqr_vec_BC_model.cpp multi_exp_Asqr_vec_BC_model.h \
		abstract_model.h \
		multi_exp_Asqr_vec_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_vec_BC_model.o multi_exp_Asqr_vec_BC_model.cpp

multi_exp_Asqr_expE_vec_BC_model.o: multi_exp_Asqr_expE_vec_BC_model.cpp multi_exp_Asqr_expE_vec_BC_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_vec_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_vec_BC_model.o multi_exp_Asqr_expE_vec_BC_model.cpp

multi_alt_exp_vec_BC_model.o: multi_alt_exp_vec_BC_model.cpp multi_alt_exp_vec_BC_model.h \
		abstract_model.h \
		multi_alt_exp_vec_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_vec_BC_model.o multi_alt_exp_vec_BC_model.cpp

multi_alt_exp_expE_vec_BC_model.o: multi_alt_exp_expE_vec_BC_model.cpp multi_alt_exp_expE_vec_BC_model.h \
		abstract_model.h \
		multi_alt_exp_expE_vec_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_vec_BC_model.o multi_alt_exp_expE_vec_BC_model.cpp

multi_alt_exp_Asqr_vec_BC_model.o: multi_alt_exp_Asqr_vec_BC_model.cpp multi_alt_exp_Asqr_vec_BC_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_vec_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_vec_BC_model.o multi_alt_exp_Asqr_vec_BC_model.cpp

multi_alt_exp_Asqr_expE_vec_BC_model.o: multi_alt_exp_Asqr_expE_vec_BC_model.cpp multi_alt_exp_Asqr_expE_vec_BC_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_vec_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_vec_BC_model.o multi_alt_exp_Asqr_expE_vec_BC_model.cpp

multi_exp_mat_model.o: multi_exp_mat_model.cpp multi_exp_mat_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_mat_model.o multi_exp_mat_model.cpp

multi_exp_mat_upper_model.o: multi_exp_mat_upper_model.cpp multi_exp_mat_upper_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_mat_upper_model.o multi_exp_mat_upper_model.cpp

multi_exp_mat_II_model.o: multi_exp_mat_II_model.cpp multi_exp_mat_II_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_mat_II_model.o multi_exp_mat_II_model.cpp

multi_exp_mat_II_upper_model.o: multi_exp_mat_II_upper_model.cpp multi_exp_mat_II_upper_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_mat_II_upper_model.o multi_exp_mat_II_upper_model.cpp

multi_exp_expE_mat_model.o: multi_exp_expE_mat_model.cpp multi_exp_expE_mat_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_mat_model.o multi_exp_expE_mat_model.cpp

multi_exp_expE_mat_upper_model.o: multi_exp_expE_mat_upper_model.cpp multi_exp_expE_mat_upper_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_mat_upper_model.o multi_exp_expE_mat_upper_model.cpp

multi_exp_expE_mat_II_model.o: multi_exp_expE_mat_II_model.cpp multi_exp_expE_mat_II_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_mat_II_model.o multi_exp_expE_mat_II_model.cpp

multi_exp_expE_mat_II_upper_model.o: multi_exp_expE_mat_II_upper_model.cpp multi_exp_expE_mat_II_upper_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_mat_II_upper_model.o multi_exp_expE_mat_II_upper_model.cpp

multi_alt_exp_mat_model.o: multi_alt_exp_mat_model.cpp multi_alt_exp_mat_model.h \
		abstract_model.h \
		multi_exp_mat_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_mat_model.o multi_alt_exp_mat_model.cpp

multi_alt_exp_expE_mat_model.o: multi_alt_exp_expE_mat_model.cpp multi_alt_exp_expE_mat_model.h \
		abstract_model.h \
		multi_exp_expE_mat_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_mat_model.o multi_alt_exp_expE_mat_model.cpp

multi_exp_nonsym_mat_model.o: multi_exp_nonsym_mat_model.cpp multi_exp_nonsym_mat_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_nonsym_mat_model.o multi_exp_nonsym_mat_model.cpp

multi_exp_expE_nonsym_mat_model.o: multi_exp_expE_nonsym_mat_model.cpp multi_exp_expE_nonsym_mat_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_nonsym_mat_model.o multi_exp_expE_nonsym_mat_model.cpp

multi_alt_exp_nonsym_mat_model.o: multi_alt_exp_nonsym_mat_model.cpp multi_alt_exp_nonsym_mat_model.h \
		abstract_model.h \
		multi_exp_nonsym_mat_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_nonsym_mat_model.o multi_alt_exp_nonsym_mat_model.cpp

multi_alt_exp_expE_nonsym_mat_model.o: multi_alt_exp_expE_nonsym_mat_model.cpp multi_alt_exp_expE_nonsym_mat_model.h \
		abstract_model.h \
		multi_exp_expE_nonsym_mat_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_nonsym_mat_model.o multi_alt_exp_expE_nonsym_mat_model.cpp

multi_exp_2var_model.o: multi_exp_2var_model.cpp multi_exp_2var_model.h \
		abstract_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_2var_model.o multi_exp_2var_model.cpp

multi_exp_expE_2var_model.o: multi_exp_expE_2var_model.cpp multi_exp_expE_2var_model.h \
		abstract_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_2var_model.o multi_exp_expE_2var_model.cpp

multi_exp_const_model.o: multi_exp_const_model.cpp multi_exp_const_model.h \
		abstract_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_const_model.o multi_exp_const_model.cpp

multi_exp_expE_const_model.o: multi_exp_expE_const_model.cpp multi_exp_expE_const_model.h \
		abstract_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_const_model.o multi_exp_expE_const_model.cpp

multi_exp_Asqr_const_model.o: multi_exp_Asqr_const_model.cpp multi_exp_Asqr_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_const_model.o multi_exp_Asqr_const_model.cpp

multi_exp_Asqr_expE_const_model.o: multi_exp_Asqr_expE_const_model.cpp multi_exp_Asqr_expE_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_const_model.o multi_exp_Asqr_expE_const_model.cpp

multi_alt_exp_const_model.o: multi_alt_exp_const_model.cpp multi_alt_exp_const_model.h \
		abstract_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_const_model.o multi_alt_exp_const_model.cpp

multi_alt_exp_expE_const_model.o: multi_alt_exp_expE_const_model.cpp multi_alt_exp_expE_const_model.h \
		abstract_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_const_model.o multi_alt_exp_expE_const_model.cpp

multi_alt_exp_Asqr_const_model.o: multi_alt_exp_Asqr_const_model.cpp multi_alt_exp_Asqr_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_const_model.o multi_alt_exp_Asqr_const_model.cpp

multi_alt_exp_Asqr_expE_const_model.o: multi_alt_exp_Asqr_expE_const_model.cpp multi_alt_exp_Asqr_expE_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_const_model.o multi_alt_exp_Asqr_expE_const_model.cpp

multi_exp_vec_const_model.o: multi_exp_vec_const_model.cpp multi_exp_vec_const_model.h \
		abstract_model.h \
		multi_exp_vec_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_vec_const_model.o multi_exp_vec_const_model.cpp

multi_exp_expE_vec_const_model.o: multi_exp_expE_vec_const_model.cpp multi_exp_expE_vec_const_model.h \
		abstract_model.h \
		multi_exp_expE_vec_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_vec_const_model.o multi_exp_expE_vec_const_model.cpp

multi_exp_Asqr_vec_const_model.o: multi_exp_Asqr_vec_const_model.cpp multi_exp_Asqr_vec_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_vec_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_vec_const_model.o multi_exp_Asqr_vec_const_model.cpp

multi_exp_Asqr_expE_vec_const_model.o: multi_exp_Asqr_expE_vec_const_model.cpp multi_exp_Asqr_expE_vec_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_vec_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_vec_const_model.o multi_exp_Asqr_expE_vec_const_model.cpp

multi_alt_exp_vec_const_model.o: multi_alt_exp_vec_const_model.cpp multi_alt_exp_vec_const_model.h \
		abstract_model.h \
		multi_alt_exp_vec_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_vec_const_model.o multi_alt_exp_vec_const_model.cpp

multi_alt_exp_expE_vec_const_model.o: multi_alt_exp_expE_vec_const_model.cpp multi_alt_exp_expE_vec_const_model.h \
		abstract_model.h \
		multi_alt_exp_expE_vec_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_vec_const_model.o multi_alt_exp_expE_vec_const_model.cpp

multi_alt_exp_Asqr_vec_const_model.o: multi_alt_exp_Asqr_vec_const_model.cpp multi_alt_exp_Asqr_vec_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_vec_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_vec_const_model.o multi_alt_exp_Asqr_vec_const_model.cpp

multi_alt_exp_Asqr_expE_vec_const_model.o: multi_alt_exp_Asqr_expE_vec_const_model.cpp multi_alt_exp_Asqr_expE_vec_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_vec_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_vec_const_model.o multi_alt_exp_Asqr_expE_vec_const_model.cpp

multi_exp_BC_const_model.o: multi_exp_BC_const_model.cpp multi_exp_BC_const_model.h \
		abstract_model.h \
		multi_exp_BC_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_BC_const_model.o multi_exp_BC_const_model.cpp

multi_exp_expE_BC_const_model.o: multi_exp_expE_BC_const_model.cpp multi_exp_expE_BC_const_model.h \
		abstract_model.h \
		multi_exp_expE_BC_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_BC_const_model.o multi_exp_expE_BC_const_model.cpp

multi_exp_Asqr_BC_const_model.o: multi_exp_Asqr_BC_const_model.cpp multi_exp_Asqr_BC_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_BC_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_BC_const_model.o multi_exp_Asqr_BC_const_model.cpp

multi_exp_Asqr_expE_BC_const_model.o: multi_exp_Asqr_expE_BC_const_model.cpp multi_exp_Asqr_expE_BC_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_BC_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_BC_const_model.o multi_exp_Asqr_expE_BC_const_model.cpp

multi_alt_exp_BC_const_model.o: multi_alt_exp_BC_const_model.cpp multi_alt_exp_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_BC_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_BC_const_model.o multi_alt_exp_BC_const_model.cpp

multi_alt_exp_expE_BC_const_model.o: multi_alt_exp_expE_BC_const_model.cpp multi_alt_exp_expE_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_expE_BC_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_BC_const_model.o multi_alt_exp_expE_BC_const_model.cpp

multi_alt_exp_Asqr_BC_const_model.o: multi_alt_exp_Asqr_BC_const_model.cpp multi_alt_exp_Asqr_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_BC_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_BC_const_model.o multi_alt_exp_Asqr_BC_const_model.cpp

multi_alt_exp_Asqr_expE_BC_const_model.o: multi_alt_exp_Asqr_expE_BC_const_model.cpp multi_alt_exp_Asqr_expE_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_BC_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_BC_const_model.o multi_alt_exp_Asqr_expE_BC_const_model.cpp

multi_exp_vec_BC_const_model.o: multi_exp_vec_BC_const_model.cpp multi_exp_vec_BC_const_model.h \
		abstract_model.h \
		multi_exp_vec_BC_model.h \
		multi_exp_vec_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_vec_BC_const_model.o multi_exp_vec_BC_const_model.cpp

multi_exp_expE_vec_BC_const_model.o: multi_exp_expE_vec_BC_const_model.cpp multi_exp_expE_vec_BC_const_model.h \
		abstract_model.h \
		multi_exp_expE_vec_BC_model.h \
		multi_exp_expE_vec_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_expE_vec_BC_const_model.o multi_exp_expE_vec_BC_const_model.cpp

multi_exp_Asqr_vec_BC_const_model.o: multi_exp_Asqr_vec_BC_const_model.cpp multi_exp_Asqr_vec_BC_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_vec_BC_model.h \
		multi_exp_Asqr_vec_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_vec_BC_const_model.o multi_exp_Asqr_vec_BC_const_model.cpp

multi_exp_Asqr_expE_vec_BC_const_model.o: multi_exp_Asqr_expE_vec_BC_const_model.cpp multi_exp_Asqr_expE_vec_BC_const_model.h \
		abstract_model.h \
		multi_exp_Asqr_expE_vec_BC_model.h \
		multi_exp_Asqr_expE_vec_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_exp_Asqr_expE_vec_BC_const_model.o multi_exp_Asqr_expE_vec_BC_const_model.cpp

multi_alt_exp_vec_BC_const_model.o: multi_alt_exp_vec_BC_const_model.cpp multi_alt_exp_vec_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_vec_BC_model.h \
		multi_alt_exp_vec_model.h \
		multi_alt_exp_model.h \
		multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_vec_BC_const_model.o multi_alt_exp_vec_BC_const_model.cpp

multi_alt_exp_expE_vec_BC_const_model.o: multi_alt_exp_expE_vec_BC_const_model.cpp multi_alt_exp_expE_vec_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_expE_vec_BC_model.h \
		multi_alt_exp_expE_vec_model.h \
		multi_alt_exp_expE_model.h \
		multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_expE_vec_BC_const_model.o multi_alt_exp_expE_vec_BC_const_model.cpp

multi_alt_exp_Asqr_vec_BC_const_model.o: multi_alt_exp_Asqr_vec_BC_const_model.cpp multi_alt_exp_Asqr_vec_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_vec_BC_model.h \
		multi_alt_exp_Asqr_vec_model.h \
		multi_alt_exp_Asqr_model.h \
		multi_exp_Asqr_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_vec_BC_const_model.o multi_alt_exp_Asqr_vec_BC_const_model.cpp

multi_alt_exp_Asqr_expE_vec_BC_const_model.o: multi_alt_exp_Asqr_expE_vec_BC_const_model.cpp multi_alt_exp_Asqr_expE_vec_BC_const_model.h \
		abstract_model.h \
		multi_alt_exp_Asqr_expE_vec_BC_model.h \
		multi_alt_exp_Asqr_expE_vec_model.h \
		multi_alt_exp_Asqr_expE_model.h \
		multi_exp_Asqr_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_alt_exp_Asqr_expE_vec_BC_const_model.o multi_alt_exp_Asqr_expE_vec_BC_const_model.cpp

threept_constant_model.o: threept_constant_model.cpp threept_constant_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_constant_model.o threept_constant_model.cpp

threept_constant_sqr_model.o: threept_constant_sqr_model.cpp threept_constant_sqr_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_constant_sqr_model.o threept_constant_sqr_model.cpp

threept_multi_exp_model.o: threept_multi_exp_model.cpp threept_multi_exp_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_exp_model.o threept_multi_exp_model.cpp

threept_multi_exp_expE_model.o: threept_multi_exp_expE_model.cpp threept_multi_exp_expE_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_exp_expE_model.o threept_multi_exp_expE_model.cpp

threept_multi_alt_exp_model.o: threept_multi_alt_exp_model.cpp threept_multi_alt_exp_model.h \
		abstract_model.h \
		threept_multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_alt_exp_model.o threept_multi_alt_exp_model.cpp

threept_multi_alt_exp_expE_model.o: threept_multi_alt_exp_expE_model.cpp threept_multi_alt_exp_expE_model.h \
		abstract_model.h \
		threept_multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_alt_exp_expE_model.o threept_multi_alt_exp_expE_model.cpp

threept_multi_exp_vec_model.o: threept_multi_exp_vec_model.cpp threept_multi_exp_vec_model.h \
		abstract_model.h \
		threept_multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_exp_vec_model.o threept_multi_exp_vec_model.cpp

threept_multi_exp_expE_vec_model.o: threept_multi_exp_expE_vec_model.cpp threept_multi_exp_expE_vec_model.h \
		abstract_model.h \
		threept_multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_exp_expE_vec_model.o threept_multi_exp_expE_vec_model.cpp

threept_multi_alt_exp_vec_model.o: threept_multi_alt_exp_vec_model.cpp threept_multi_alt_exp_vec_model.h \
		abstract_model.h \
		threept_multi_alt_exp_model.h \
		threept_multi_exp_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_alt_exp_vec_model.o threept_multi_alt_exp_vec_model.cpp

threept_multi_alt_exp_expE_vec_model.o: threept_multi_alt_exp_expE_vec_model.cpp threept_multi_alt_exp_expE_vec_model.h \
		abstract_model.h \
		threept_multi_alt_exp_expE_model.h \
		threept_multi_exp_expE_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o threept_multi_alt_exp_expE_vec_model.o threept_multi_alt_exp_expE_vec_model.cpp

multi_part_exp_expE_model.o: multi_part_exp_expE_model.cpp multi_part_exp_expE_model.h \
		abstract_model.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o multi_part_exp_expE_model.o multi_part_exp_expE_model.cpp

.PHONY : clean
clean :
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~
