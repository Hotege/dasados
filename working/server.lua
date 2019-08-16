local function table2json(t, comma)
    local res = ""
    if (type(t) == "table") then
        res = res .. "{"
        local count = 0 
        for k, v in pairs(t) do
            count = count + 1 
        end 
        local id = 1 
        for k, v in pairs(t) do
            res = res .. "\""
            res = res .. k
            res = res .. "\":"
            res = res .. table2json(v, id < count)
            id = id + 1 
        end 
        res = res .. "}" 
        if (comma) then
            res = res .. "," 
        end 
    end 
    if (type(t) == "string") then
        res = res .. "\""
        res = res .. t
        res = res .. "\""
        if (comma) then
            res = res .. "," 
        end 
    end 
    if (type(t) == "number") then
        res = res .. t
        if (comma) then
            res = res .. "," 
        end
    end
    return res
end

local function encodeBase64(source_str)
    local b64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
    local s64 = ''
    local str = source_str
    while #str > 0 do
        local bytes_num = 0
        local buf = 0
        for byte_cnt=1,3 do
            buf = (buf * 256)
            if #str > 0 then
                buf = buf + string.byte(str, 1, 1)
                str = string.sub(str, 2)
                bytes_num = bytes_num + 1
            end
        end
        for group_cnt=1,(bytes_num+1) do
            local b64char = math.fmod(math.floor(buf/262144), 64) + 1
            s64 = s64 .. string.sub(b64chars, b64char, b64char)
            buf = buf * 64
        end
        for fill_cnt=1,(3-bytes_num) do
            s64 = s64 .. '='
        end
    end
    return s64
end

local handlers = {}

local function loadFile(filename)
    local file = io.open(filename, "rb")
    local len = file:seek("end")
    file:seek("set")
    local data = file:read(len)
    io.close(file)
    return len, data
end

local function getStaticFile(conn, t)
    local filename = t["filedir"] .. "/" .. string.sub(t["dir"], 7, #t["dir"])
    local len, data = loadFile(filename)
    local header = "HTTP/1.1 200 OK\r\nContent-Length: " .. len .. "\r\n\r\n"
    sendBuffer(conn, header, #header)
    sendBuffer(conn, data, len)
end

handlers["/favicon.ico"] = function(conn, t, template)
    local filename = t["filedir"] .. "/" .. "favicon.ico"
    local len, data = loadFile(filename)
    local header = "HTTP/1.1 200 OK\r\nContent-Length: " .. len .. "\r\nContent-Type: image/x-icon\r\n\r\n"
    sendBuffer(conn, header, #header)
    sendBuffer(conn, data, len)
end

handlers["/"] = function(conn, t, template)
    if (type(t["params"]["id"]) == "nil") then
        t["params"]["id"] = "0"
    end
    local header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
    local modules = getSelectAll("module")
    if (type(modules[t["params"]["id"]]) == "nil") then
        local l, errorPage = loadFile(t["filedir"] .. "/" .. "templates/error.html")
        local pattern = "{{(%s*).error(%s*)}}"
        local html = string.gsub(errorPage, pattern, function(p)
            return "Cannot find module."
        end)
        local response = header .. html
        sendBuffer(conn, response, #response)
        return
    end
    local articles = getSelectAll("article")
    local data = {}
    data["renderType"] = "module"
    data["renderId"] = t["params"]["id"]
    data["modules"] = modules
    data["articles"] = articles
    local dataString = table2json(data)
    local pattern = "{{(%s*).data(%s*)}}"
    local html = string.gsub(template, pattern, function(p)
        return encodeBase64(dataString)
    end)
    local response = header .. html
    sendBuffer(conn, response, #response)
end

handlers["/module"] = function(conn, t, template)
    if (t["params"]["id"] == "0") then
        response = "HTTP/1.1 302 Found\r\nLocation: /\r\n\r\n"
        sendBuffer(conn, response, #response)
        return
    end
    handlers["/"](conn, t, template)
end

handlers["/article"] = function(conn, t, template)
    local modules = getSelectAll("module")
    local articles = getSelectAll("article")
    local data = {}
    data["renderType"] = "article"
    data["modules"] = modules
    data["articles"] = articles
    local len, readData = loadFile(t["filedir"] .. "/" .. articles[t["params"]["id"]]["path"])
    data["articleData"] = encodeBase64(readData)
    local dataString = table2json(data)
    local pattern = "{{(%s*).data(%s*)}}"
    local html = string.gsub(template, pattern, function(p)
        return encodeBase64(dataString)
    end)
    local header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
    local response = header .. html
    sendBuffer(conn, response, #response)
end

function resolveDirectory(conn, t)
    if (string.sub(t["dir"], 1, 6) == "/file/" and #t["dir"] > 6) then
        getStaticFile(conn, t);
    end
    if (type(handlers[t["dir"]]) ~= "nil") then
        local len, template = loadFile(t["filedir"] .. "/" .. "templates/template.html")
        handlers[t["dir"]](conn, t, template)
    else
        local l, errorPage = loadFile(t["filedir"] .. "/" .. "templates/error.html")
        local pattern = "{{(%s*).error(%s*)}}"
        local html = string.gsub(errorPage, pattern, function(p)
            return "Cannot find page " .. t["dir"] .. "."
        end)
        local header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        local response = header .. html
        sendBuffer(conn, response, #response)
    end
end
