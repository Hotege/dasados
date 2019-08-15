function utf8to16(str) {
    var out, i, len, c;
    var char2, char3;
    out = "";
    len = str.length;
    i = 0;
    while(i < len) {
        c = str.charCodeAt(i++);
        switch(c >> 4) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            out += str.charAt(i-1);
            break;
        case 12: case 13:
            char2 = str.charCodeAt(i++);
            out += String.fromCharCode(((c & 0x1F) << 6) | (char2 & 0x3F));
            break;
        case 14:
            char2 = str.charCodeAt(i++);
            char3 = str.charCodeAt(i++);
            out += String.fromCharCode(((c & 0x0F) << 12) | ((char2 & 0x3F) << 6) | ((char3 & 0x3F) << 0));
            break;
        }
    }
    return out;
}

window.onload = function() {
    document.charset = 'gb2312';
    var render = document.getElementById('render');
    var data = render.getAttribute('data');
    var code = utf8to16(window.atob(data));
    var obj = JSON.parse(code);

    if (obj.renderType == 'module') {
        console.log(obj);

        var d = document.createElement('div');
        d.innerHTML = code;
        document.body.appendChild(d);
    }
    if (obj.renderType == "article") {
        var articleCode = utf8to16(window.atob(obj.articleData));
        var articleObj = JSON.parse(articleCode);

        console.log(obj);
        console.log(articleObj);

        var d = document.createElement('div');
        d.innerHTML = articleCode;
        document.body.appendChild(d);
    }
}
