window.onload = function() {
    var render = document.getElementById('render');
    var data = render.getAttribute('data');
    var code = window.atob(data);
    var obj = JSON.parse(code);

    if (obj.renderType == 'module') {
        var d = document.createElement('div');
        d.innerHTML = code;
        document.body.appendChild(d);

        console.log(obj);
    }
    if (obj.renderType == "article") {
        var articleCode = window.atob(obj.articleData);
        var articleObj = JSON.parse(articleCode);

        console.log(obj);
        console.log(articleObj);
    }
}
