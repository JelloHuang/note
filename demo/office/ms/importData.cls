VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True'
END

'����һ��text �ļ���excel��'
'��Ϥ�����ı��ļ�'
Sub AddQuery()
    Sheet1.UsedRange.ClearContents
    With Sheet1.QueryTables.Add( _
        Connection:="TEXT;" & ThisWorkbook.Path & "\acknowledges.txt", _
        Destination:=Range("A1"))
        .TextFilePlatform = 936
        .TextFileCommaDelimiter = True
        .Refresh
    End With
End Sub

'����һ���û�ѡ���Ŀ¼, �����������ļ���Ϣд�뵽excel ��'
'��ϤĿ¼�Ĳ���'
Sub getpath()
	'���A2��C1000��'
    Range("A2:C1000").ClearContents              
    On Error Resume Next
    Dim shell As Variant
    Set shell = CreateObject("Shell.Application")
	'��ȡ�ļ���·����ַ �ֶ�ѡ��'
    Set filePath = shell.BrowseForFolder(&O0, "ѡ���ļ���", &H1 + &H10, "")   
    Set shell = Nothing
  '����Ƿ�����Ч·������ȡ��ֱ����������'
  If filePath Is Nothing Then                 
       Exit Sub
    Else
       gg = filePath.Items.Item.Path
  End If
    Set obj = CreateObject("Scripting.FileSystemObject")
	'��ȡ·��'
    Set fld = obj.getfolder(gg)                         
    m = 0
    Cells(1, 1) = "Name"
    Cells(1, 2) = "Path"
    Cells(1, 3) = "Size"
    Cells(1, 4) = "DateCreated"
	 '�����ļ������ļ�'
     For Each ff In fld.Files                   
        m = m + 1
        Cells(m + 1, 1) = ff.Name
        Cells(m + 1, 2) = ff.Path
        Cells(m + 1, 3) = ff.Size
        Cells(m + 1, 4) = ff.DateCreated
      Next
End Sub

Sub ImportData()
    On Error Resume Next
    Dim shell As Variant
    Set shell = CreateObject("Shell.Application")
	'��ȡ�ļ���·����ַ �ֶ�ѡ��'
    Set filePath = shell.BrowseForFolder(&O0, "ѡ���ļ���", &H1 + &H10, "")   
    Set shell = Nothing
  '����Ƿ�����Ч·������ȡ��ֱ����������'
  If filePath Is Nothing Then                 
       Exit Sub
  Else
       gg = filePath.Items.Item.Path
  End If
	'�������'
    Set obj = CreateObject("Scripting.FileSystemObject")   
	'��ȡ·��'
    Set fld = obj.getfolder(gg)                         
   
	'�����ļ������ļ�'
    For Each ff In fld.Files                   
        If Worksheets(ff.Name) Is Nothing Then
            Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = ff.Name
        Else
			'ɾ��ʱ����ȷ��'
            Application.DisplayAlerts = False 
            Worksheets(ff.Name).Delete
            Worksheets.Add(After:=Worksheets(Worksheets.Count)).Name = ff.Name
        End If
      
        With Worksheets(ff.Name).QueryTables.Add(Connection:="TEXT;" & ff.Path, Destination:=Range("A1"))
            .TextFilePlatform = 936
            .TextFileCommaDelimiter = True
            .Refresh
        End With
    Next
End Sub
