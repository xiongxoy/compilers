package orz.school.compiler;

import java.util.HashMap;


public class LexAnalyser {
	
	static final int  PLUS = 0;
	static final int  MINUS = 1;
	static final int  MUL = 2;
	static final int  DIV = 3;
	static final int  L_BRAC = 4;
	static final int  R_BRAC = 5;
	static final int  EQUAL = 6;
	static final int  INT = 7;
	static final int  FLOAT = 8;
	static final int  ID = 9;
	static final int  SEMICO = 10;
	static final int  COMMA = 11;
	static final int  UNDER_SC = 12;
	static final int  KEY_INT = 22;
	static final int  KEY_FLOAT = 23;
	static final String keyword[] = { "int", "float"};
	static final String Symbol[] = { "+", "-", "*", "/", "=",  ";", ",", "(", ")" ,"_"};
	
	/*
    FileReader fin = new FileReader("XXX");
    BufferedReader txtfile = new BufferedReader( fin );
 
    FileWriter fout = new FileWriter("XXX");
    BufferedWriter table = new BufferedWriter(fout);
    fout.close();
 	*/
	public static String analysis(String txtAnalysis, HashMap<String, SymEntry> symboleTable) {
		int lenth = 0; // 字符数组指针
        int txtLength = txtAnalysis.length();
        String strtemp = new String();
        String txtResult = "";
        char[] textchar = txtAnalysis.toCharArray(); // 分解为字符数组
		
         while (lenth < txtLength) {
			if (Character.isWhitespace(textchar[lenth])) // 若为空格，则取下一个字符
				lenth++;
			else if (Character.isDigit(textchar[lenth])) // 识别常数
			{
				boolean IsFloat = false; // 标识浮点数
				int state=0;
				strtemp = "";
									
				// TODO need to check length
state_loop:		while(true)  //识别常数的自动机
				{
					if(lenth >= txtLength)
					{
						state = 7;
					}
					switch(state){
					case 0: if(Character.isDigit(textchar[lenth])){
						strtemp += String.valueOf(textchar[lenth++]);
						state=1;
					}
					else{
						System.err.println("不是数字");
						System.exit(-1);
					}
					break;
					case 1: 
						if(Character.isDigit(textchar[lenth])){
							strtemp += String.valueOf(textchar[lenth++]);
							state=1;
						}
						else if(textchar[lenth] == '.'){
							strtemp += String.valueOf(textchar[lenth++]);
							IsFloat = true;
							state=2;
						}
						else if(textchar[lenth] == 'e' || textchar[lenth] == 'E'){
							strtemp += String.valueOf(textchar[lenth++]);
							IsFloat = true;
							state=4;
						}
						else
							state=7;
						break;
					case 2:
						if(Character.isDigit(textchar[lenth])){
							strtemp += String.valueOf(textchar[lenth++]);
							state=3;
						}
						else{
							System.out.println(state);
							System.err.println("数字格式错误");
							System.exit(-1);
						}
						break;
					case 3:
						if(Character.isDigit(textchar[lenth])){
							strtemp += String.valueOf(textchar[lenth++]);
							state=3;
						}
						else if(textchar[lenth] == 'e' || textchar[lenth] == 'E'){
							strtemp += String.valueOf(textchar[lenth++]);
							IsFloat = true;
							state=4;
						}
						else
							state=7;
						break;
					case 4:
						if(Character.isDigit(textchar[lenth]) ||
						   textchar[lenth] == '+' ||
						   textchar[lenth] == '-'){
							strtemp += String.valueOf(textchar[lenth++]);
							state=5;
						}
						else{
							System.err.println("数字格式错误");
							System.exit(-1);
						}
						break;
					case 5:
						if(Character.isDigit(textchar[lenth])){
							strtemp += String.valueOf(textchar[lenth++]);
							state=6;
						}
						else{
							System.err.println("数字格式错误");
							System.exit(-1);
						}
						break;
					case 6:
						if(Character.isDigit(textchar[lenth])){
							strtemp += String.valueOf(textchar[lenth++]);
							state=6;
						}
						else
							state=7;
						break;
					case 7: // OK!
						if(IsFloat) {
							txtResult = txtResult + FLOAT + "\t"
										+  strtemp+ "\n";
						}
						else{
							txtResult = txtResult + INT + "\t"
									+  strtemp+ "\n";
						}
						break state_loop;
					}
				}
			} else if (Character.isLetter(textchar[lenth]) || 
					   textchar[lenth] == '_') // 处理字符的情况
			{
				// 识别关键词
				strtemp = "";
				while (lenth < txtLength  // doesn't need ReadLine
						&& (Character.isLetter(textchar[lenth]) ||
							Character.isDigit(textchar[lenth]) ||
							textchar[lenth] == '_'))  {
					strtemp = strtemp + String.valueOf(textchar[lenth++]);
				}
				if (strtemp.equals(keyword[0])) {
					txtResult = "";
					String sym = txtAnalysis.substring(txtAnalysis.lastIndexOf(" ")+1,
							 txtAnalysis.indexOf("="));
					String value = txtAnalysis.substring(txtAnalysis.indexOf("=")+1,
														 txtAnalysis.indexOf(";"));
					SymEntry entry = new SymEntry("int",value);
					symboleTable.put(sym, entry);
					break;
				}
				else if (strtemp.equals(keyword[1])) {
					txtResult = "";
					String sym = txtAnalysis.substring(txtAnalysis.lastIndexOf(" ")+1,
							 txtAnalysis.indexOf("="));
					String value = txtAnalysis.substring(txtAnalysis.indexOf("=")+1,
														 txtAnalysis.indexOf(";"));
					SymEntry entry = new SymEntry("float",value);
					symboleTable.put(sym, entry);
					break;
				}
				else //描述符（ID）
				{
					if(!symboleTable.containsKey(strtemp))
					{
						System.err.println("No Symblole: " + strtemp);
						//symboleTable.put(strtemp,new SymEntry("int"));
					}
					txtResult = txtResult + ID + "\t"
								+ strtemp  + "\n";
				}
			} else if (textchar[lenth] == '+') {
				txtResult = txtResult + PLUS + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == '-') {
				txtResult = txtResult + MINUS + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == '*') {
				txtResult = txtResult + MUL + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == '=') {
				txtResult = txtResult + EQUAL + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == ';') {
				txtResult = txtResult + SEMICO + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == '(') {
				txtResult = txtResult + L_BRAC + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == ')') {
				txtResult = txtResult + R_BRAC + "\t"
						+  textchar[lenth++] + "\n";
			} else if (textchar[lenth] == '/') { // TODO 加入识别注释的功能
				txtResult = txtResult + DIV + "\t"
						+  textchar[lenth++] + "\n";
			} else {
				txtResult = txtResult + "非法字符" + "\t"
						+  textchar[lenth++] + "\n";
			}
		}
         return txtResult;
	}
		
}






