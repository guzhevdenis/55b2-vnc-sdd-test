#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Author        : Janez Golob (Cosylab d.d)
#
# Copyright (c) : 2010-2017 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

SUBDIRS=$(dir $(wildcard */Makefile))

BOLD=\e[1m
NC=\e[0m

%:
	echo -e "$(BOLD)Building Tests...$(NC)"
	@set -e; for ii in $(SUBDIRS); do \
		echo -e "$(BOLD)Building $$ii...$(NC)"; \
		$(MAKE) $@ -C $$ii; \
	done

all:
