Document Object Model�ĵ�����ģ��

CSS+JavaScript+DOM=DHTML

��ʱ setInterval  
ÿ���һ��ʱ��ִ��
```
intervalId=setInterval("alert('hello')", 5000)//ÿ��5000����,Ҳ����5�뵯��һ��hello�Ի���
```

setTimeout

clearTimeout

navigate

`<input type="button" onclick="navigate('HTMLPageForm.htm')" value="navigate demo" />`

navigate("http://www.baidu.com")
confirm()

```
if (confirm("�Ƿ�Ҫ����������ı�")) {
	alert("yes");
}
else{
	alert("no");
}
```
confirm()����������ֹ�û�������������뿪���ҳ�����ʹ����Ķ�������ʧ��
 
## body,document������¼�
- onload:��ҳ������Ϻ󴥷�
- onunload:��ҳ�ر�(�����뿪)�󴥷�
- onbeforeunload:����ҳ׼���ر�(�����뿪)ʱ����
- onbeforeunload="window.event.returnValue='�ı��ᶪʧ'"
- onclick ����
- ondblclick˫��
- onkeydown��������
- onkeypress�������
- onkeyup�����ͷ�
- onmousedown��갴��
- onmousemove����ƶ�
- onmouseout����뿪Ԫ�ط�Χ
- onmouseover����ƶ���Ԫ�ط�Χ
- onmouseup��갴���ͷ�
- onmousedown :���������ʱ�������¼�
- ��ַlocation

window.location.href
�ȿ���ͨ����ȡ�õ�ǰ��ַ,Ҳ���Ը�����ֵʵ��navigate��Ч��

window.location.reload()ˢ��

window.event ����¼�����ʱ����Ϣ,ʱ�䲻������window������¼�.������winForm�е�e(EventArg)

���ⰴ�����ж�
altKey����,bool����,��ʾ�����¼�ʱalt���ǰ���.���ƵĻ���ctrlKey,shiftKey����
bool window.event.altKey
```
<input type="button" value="���" onclick="if(window.event.ctrlKey){alert('�����ctrl��');};else {alert('û�е��ctrl��');}" />
```

����

clientX ,clientY�����¼�ʱ����ڿͻ���������

screen X, screen Y�����¼�ʱ�������Ļ�ϵ�����
offsetX,offsetY�����¼�ʱ���������¼�Դ(��������ťʱ����onclick)������

returnValue����,�����������Ϊfalse,�ͻ�ȡ��Ĭ���¼��Ĵ���
```
<a href="HTMLPageTable.htm" onclick="alert('��ֹ����');window.event.returnValue=false;">hi</a>
```
�����û���ע��ʱ,������Ϣ����,���ʱ��Ͳ����ύ��������,�Ϳ���ͨ�������ʽ

 
srcElement ����¼�Դ����

keyCode�����¼�ʱ�İ���ֵ
button�����¼�ʱ����갴��,1Ϊ���,2Ϊ�Ҽ�,3Ϊ���Ҽ�ͬʱ��

 
screen����

screen.width;screen.height

clipboardData����

��ճ����Ĳ���
�����κ�Ԫ�ض���oncopy��onpaste��������

clearData("Text")���ճ����,����ֻ��ʱText(ע:������Ϊ���Ժ����չ������Ҫ��)
getData("Text")��ȡճ�����ֵ,����ֵΪճ�����е�����

setData("Text",val)����ճ�����е�ֵ
 
```
<body oncopy="alert('��ֹ����');window.event.returnValue=false;"></body>
<body oncopy="alert('��ֹ����'); return false;"></body>
 
mobile:<input type="text" /><br />
mobile again:<input type="text" onpaste="alert('Ϊ��֤����ʽ��ֵ����ȷ���ֻ���,������');return false;" />

function modifyClipboard(){
	clipboardData.setData('Text', clipboardData.getData('Text') +"����ת��:"+ window.location.href);
}
<body oncopy="setTimeout('modifyClipboard()', 100)"></body>
```

- window.history
- window.history.back();  window.history.go(-1);����һ��ҳ��
- window.history.forward();  window.history.g0(1);
- window.history.go(2);
```
<a href="javascript:window.history.back()">back</a>

Ҳ���Էŵ�input button��onclick�¼���
<input type="button" onclick="window.history.back()" value="back"  />
```

window.document

write(text);

���ĵ���д������

ֻ����ҳ����ع����д���

window.document.write("this is a document<br/>");
window.document.write("<a href='http://www.buaa.edu.cn'>buaa</a><br/>");

���������onclick�¼���,д������ݻ���ҳ���е�ԭ������

<input type="button" value="document.write" onclick="window.document.write('buaa is mine');"/>
write�����ڹ�����/������Դ������ʹ��,�������ðٶȵ�����http://news.baidu.com/newscode.html
 
getElementById();

alert(form1.textbox2.value);
alert(textbox1.value);

var txt = document.getElementById("textbox2");
alert(txt.value);

getElementsByName();

һ��ҳ����name���Բ�Ψһ,���Է��صĽ����һ������
```
<head>
	<title></title>
	   <script type="text/javascript">
			  function btnclick() {
				  var radios = document.getElementsByName("gender");
				  for (var i = 0; i < radios.length; i++) {
					   alert(radios[i].value);
				  }
			  }
	   </script>
</head>

<body>
<input type="radio" name="gender"  value="��"/>��
<input type="radio" name="gender" value="Ů" />Ů
<input type="button" value="get the gender"  onclick="btnclick()"/>
</body>
```
 
parseInt(string,radix); ��stringת��������,radixΪ����

createElement ��̬���ӵ�����ͨ���鿴��ҳԴ����ķ�ʽ�ǿ�������,����ͨ��debugbar�鿴 
```
var div = document.getElementById("divMain");
var input = document.createElement("input");
input.type = "button";
input.value = "je suis dynamique";
div.appendChild(input);

var input = document.createElement("<input type='button' value='hello'/>");
document.body.appendChild(input);
var label = document.createElement("label");
label.innerText = "user";
label.setAttribute("for", "username");//username��һ���ı���,��Ϊfor��js����һ���ؼ���,����ֱ��ʹ��label.for="username");�ǲ��е�
document.body.appendChild(label);
```

setAttribute

setAttribute����������һЩhtml����û�е�����, ����:
```
label.setAttribute("studentId", "39241715");//�����������Ժ�ʹ��
var id=label.getAttribute(("studentId");//�����Ϳ���ȡ�����ֵ
```
innerText innerHTML: �������е�DOMԪ�ض���innerText��innerHTML����Ԫ��,������Ԫ���Ǵ�Сд���е�,�ҿ��Զ���д

firefox��֧��innerText,������Ժ�����Ҫʹ��jquery��ԭ��֮һ
The innerText property is valid for block elements only. By definition, elements that do not have both an opening and closing tag cannot have an innerText property.

����innerHTML ����,�������е�Ԫ�ض���innerHTML����,����һ���ַ���,�������û��ȡλ�ڶ�����ʼ�ͽ�����ǩ�ڵ�HTML.
firefox��innerText��target
 
this ����¼�����

�޸�����

css��background-color,��js����backgroundcolor,��Ϊ��js��,-������ĺ���,����

onfocus onblur �������ؼ����¼���onfocus,�����뿪�ؼ����¼���onblur
```
<head>
	<title></title>
	   <script type="text/javascript">
			  function initEvent() {
					 var inputs = document.getElementsByTagName("input");
					 for (var i = 0; i < inputs.length; i++) {
							var input = inputs[i];
							input.onblur = inputOnblur;
					 }
			  }

			  function inputOnblur() {
					 if (this.value.length <= 0) {
							this.style.background = "red"; //�����ǽ��ؼ���onblur��Ӧ��������ΪinputOnblur,�����Ǹÿؼ���onblur��Ӧ��������inputOnblur(),�������������this			   }
					 else {
							this.style.background = "white";
					 }
			  }
	   </script>
</head>

<body onload="initEvent()">
	   <input type="text" name="name" value="content1" onblur="alert('��һ��ʧȥ�˽���')"  onfocus="alert('��һ���õ��˽���')"/>
	   <input type="text" name="name" value="content2" onblur="alert('�ڶ���ʧȥ�˽���')"  onfocus="alert('�ڶ����õ��˽���')"/>
	   <br />
	   <input type="text" /><input type="text" /><input type="text" /><input type="text" /><input type="text" />
</body>
```

onchange

onchangeֻ���ڹ���뿪ʱ��ִ��
�����Ǻ�����,�������һ��text�ı���,�������ݵ�ʱ��,���������һֱ�ڸı�,���һ�ı�ʹ���,��ôonchange����¼��ͻ�һֱ���ڴ���״̬.

style

style.display����

���Ʋ���ʾ���

ֻ�е�ֵΪnoneʱ,�Ų���ʾ,����ֵ(������ֵ)����ʾ

��IE�м���body��onclick,���һ��Ԫ�����������ǲ���������,���һ��Ҫ����,��Ҫ����document��onclick�¼�,��javascript�����
�����������������
`document.onclick = function(){alert("OK");}`

postion
дĳ������ľ�������֮ǰ,��Ҫȷ�����������͵�position

absolute ���ɾ��Զ�λ��Ԫ��,����� static ��λ����ĵ�һ����Ԫ�ؽ��ж�λ.

fixed ���ɾ��Զ�λ��Ԫ��,�������������ڽ��ж�λ.

relative ������Զ�λ��Ԫ��,�����������λ�ý��ж�λ.

static Ĭ��ֵ.û�ж�λ,Ԫ�س���������������(���� top, bottom, left, right ���� z-index ����).

inherit �涨Ӧ�ôӸ�Ԫ�ؼ̳� position ���Ե�ֵ.

style="position:absolute;top:200px;left:200px"

form submit
ͨ��һ����ͨ�İ�ťҲ���ύ����
```
<form action="default.aspx" method="post" id="form1">
	<input type="button" name="name" value="same as submit" onclick="form1.submit()" />
	<input type="submit" />
</form>
```
����ύ֮ǰҪ�������,������form��`onsubmit`(�����return false;�Ϳ�����ֹ�ύ)�¼���д����������ݼ��,
�������ͨ��`form1.submit()`�����ύ,������ִ��onsubmit��Ĵ���.

����,���onsubmit�¼�Ҫ���ú���,Ҫ�ر�ע�ⷵ��ֵ�Ĵ���
```
function beforeSubmit(){
   var name = document.getElementById("name");
   if (name.value.length <= 0) {
   		alert('name can not be empty');
		return false;
   }
}
onsubmit="return beforeSubmit()"
```

ͨ��һ����ť�ĵ����������һ����ť�������
```
<input type="button" id="btn1" value="btn" onclick="alert('btn')" />
<input type="button" name="name" value="clickonme"  onclick="btn1.click()" />
//����btn1.onclick();
```
