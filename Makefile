CC := gcc
CFLAGS := -Wall -Wextra -g -std=gnu99
OBJDIR := obj

SEND_SRCS = msg-define.c msg-send.c
SEND_OBJS = $(addprefix $(OBJDIR)/, $(SEND_SRCS:.c=.o))

RECV_SRCS = msg-define.c msg-recv.c
RECV_OBJS = $(addprefix $(OBJDIR)/, $(RECV_SRCS:.c=.o))

all: $(OBJDIR) send recv

send: $(SEND_OBJS)
	@echo "Making send executable: "$@
	@$(CC) $^ -o $@ 

recv: $(RECV_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@ 

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	@rm -vf ${SEND_OBJS}
	@rm -vf ${RECV_OBJS}
	@rm -vf send
	@rm -vf recv
	@rm -rf ${OBJDIR}
	@ipcrm -a
	@echo "Removed object files and executables..."

.PHONY: clean
