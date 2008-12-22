System.IO.Console = {
    write: function (text) {
        System.IO.write(System.IO.STDOUT, text);
    },

    writeLine: function (text) {
        System.IO.write(System.IO.STDOUT, text+"\n");
    }
}
