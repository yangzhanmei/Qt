#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainLayout = new QVBoxLayout;
    widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    //setStyleSheet("background-color:rgb(192,192,192)");

    title = new QLabel("Encode/Decode System");
    title->setStyleSheet("font-size:25px");
    mainLayout->addWidget(title);
    title->setAlignment(Qt::AlignHCenter);

    open = new QToolButton;
    open->setText("Click to choose a file...");
    open->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    connect(open,SIGNAL(clicked()),this, SLOT(chooseFile()));
    open->setFixedSize(380,30);


    encode = new QToolButton;
    encode ->setText("Encode/Decode");
    encode->setStyleSheet("color:white;background-color:rgba(1,1,1,1);border-style:inset");
    encode->setFixedHeight(30);
    connect(encode,SIGNAL(clicked()),this,SLOT(code()));


    middleLayout = new QHBoxLayout;
    middleLayout->addWidget(open);
    middleLayout->addWidget(encode);
    mainLayout->addLayout(middleLayout);


    mainLayout->addStretch(0);


}

MainWindow::~MainWindow()
{

}
void MainWindow::chooseFile()
{
    QFileDialog *fd = new QFileDialog;


    fileName = fd->getOpenFileName();

    if(!fileName.isEmpty())
        open->setText(fileName);
}
void MainWindow::code()
{
    qDebug()<<"code";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            QMessageBox::information(this,"Error","Can't open the file!");
            return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    qDebug()<<text;
    file.close();
    if(text.contains('#'))
    {
        QStringList temp = text.split("#");
        QString head = temp.at(0);
        text = temp.at(1);
        QLabel fhl("File Head Length: "+QString::number(head.length()));
        QLabel ADL("Actual Data Length: "+QString::number(text.length()));



    }
    else
    {
        qDebug()<<"else";
        QLabel *fl = new QLabel("File Length: "+QString::number(text.length()));
        HuffTree<QChar>** TreeArray = new HuffTree<QChar>*[26];
        int count=0;

        for(int i=0;i<26;i++)
        {
            if(text.contains(QChar(i+97)))
            {
                QChar ch(i+97);
                qDebug()<<ch;
                int countLetter = text.count(QString(ch));
                qDebug()<<countLetter;
                TreeArray[i] = new HuffTree<QChar>(ch,countLetter);
                qDebug()<<TreeArray[i]->weight();
                count++;
            }


        }
        qDebug()<<"TreeArray";
        qDebug()<<TreeArray[0]->weight();
        HuffTree<QChar>* tree = buildHuff(TreeArray,count);
        qDebug()<<"buildHuff";
        HuffNode<QChar>* root = tree->root();
        qDebug()<<"root";
        QString str;
        traverse(root,str);
        str.clear();



        for(int i=0;i<list.length();i++)
        {
            str+=list.at(i);
            QString temp = list.at(i);
            QString letter = temp.at(temp.length()-1);
            temp.chop(1);
            text.replace(letter,temp);
        }

        str+="#";
        str+=text;
        qDebug()<<str;

        qDebug()<<fileName;
        QString outputFileName(fileName);
        outputFileName+=".encode";
        QFile outputFile(outputFileName);


        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this,"Error","Can't create the encode file!");
            return;
        }

        qDebug()<<"open";
         QTextStream out(&outputFile);
         out<<str;

         mainLayout->addWidget(fl);
         QString dir("Encode    ");
         dir+="<a href = ";
         dir += outputFileName;
         dir += ">";
         dir += outputFileName;
         dir +="</a>";
         QLabel *fileDir = new QLabel(dir);
         connect(fileDir,SIGNAL(linkActivated(QString)),this,SLOT(openCodeFile(QString)));

         mainLayout->addWidget(fileDir);


    }

}
template <typename E> HuffTree<E>* MainWindow::buildHuff(HuffTree<E>** TreeArray, int count) {
    HuffTree<QChar> *temp1, *temp2, *temp3 = NULL;

    while (count > 1) {

        int i = getMin(TreeArray,count);
        qDebug()<<"while";
        temp1 = TreeArray[i];
        remove(TreeArray,count,i);
        count--;

        qDebug()<<temp1->weight();
        i = getMin(TreeArray,count);
        temp2 = TreeArray[i];
        remove(TreeArray,count,i);
        count--;

        temp3 = new HuffTree<E>(temp1, temp2);
        TreeArray[count] = temp3;
        count++;
        delete temp1;

        delete temp2;
    }
    return temp3;
}
template <typename E> void MainWindow::traverse(HuffNode<E>* root,QString str)
{
    if(root->isLeaf())
    {
        str+=dynamic_cast<LeafNode<E>* >(root)->val();
        list.append(str);
        str.chop(2);
        return;
    }
    str+="0";
    traverse(dynamic_cast<IntlNode<E>* >(root)->left(),str);
    str+="1";
    traverse(dynamic_cast<IntlNode<E>* >(root)->right(),str);
    str.chop(1);
    return;

}
template <typename E> int MainWindow::getMin(HuffTree<E>** TreeArray, int count)
{
    int start = 0;
    for(int i=1;i<count;i++)
    {
        if(TreeArray[i]->weight()<TreeArray[start]->weight())
        {
            start = i;
        }
    }
    return start;
}
template <typename E> void MainWindow::remove(HuffTree<E>** TreeArray, int count,int i)
{
        for(int j=i+1;j<count;j++)
        {
            TreeArray[j-1]=TreeArray[j];
        }
}
void MainWindow::openCodeFile(QString filename)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}